#include <functional>
#include <chrono>
#include "session.h"

using namespace std::placeholders;

namespace tcpAsio
{
    session::session(sock_ptr sock,io_service& ser,uint64_t sessionId,tNotify* tn):m_sock(sock),m_ios(ser),m_sessionId(sessionId),m_notify(tn)
    {
        m_read_buffer = make_shared<micro_buffer>(readBufSize);
        m_write_cell = make_shared<send_cell>();
        m_heartbeat_send = make_shared<deadline_timer>(m_ios,boost::posix_time::milliseconds(heartBeatTime));
        m_heartbeat_handle = make_shared<deadline_timer>(m_ios,boost::posix_time::milliseconds(heartBeatTime));
        m_is_connected.store(1);
        m_send_cell_is_sending.store(0);
        m_heartbeat_send_ph.size = 0;
        m_heartbeat_send_ph.type = heartbeat_server;

        //set socket buffer;
        boost::asio::socket_base::receive_buffer_size rsize(tcpRecvBufSize);
        (*m_sock).set_option(rsize);
        boost::asio::socket_base::send_buffer_size ssize(tcpSendBufSize);
        (*m_sock).set_option(ssize);

        m_notify->OnConnect(m_sessionId,(*sock).remote_endpoint().address().to_string().c_str(),(*m_sock).remote_endpoint().port());
    }

    session::~session()
    {
        handle_close();
    }

    void session::start()
    {
        deliver(m_heartbeat_send_ph,nullptr);
        (*m_heartbeat_handle).expires_from_now(boost::posix_time::milliseconds(heartBeatTime));
        (*m_heartbeat_handle).async_wait(bind(&session::heart_beat_handle,this,_1));
        do_read();
    }

    void session::do_read(){
        if(m_is_connected.load()){
            (*m_sock).async_read_some(boost::asio::buffer((*m_read_buffer).write_pos(),(*m_read_buffer).enable_write()),
                bind(&session::handle_read,this,_1,_2));
        }  
    }
    void session::handle_read(const boost::system::error_code & err, size_t bytes){
        if(!err){
            (*m_heartbeat_handle).cancel();
           (*m_read_buffer).has_write(bytes);
           while ((*m_read_buffer).enable_read() >= package_head_len)
           {
               package_head* ph = (package_head*)(*m_read_buffer).read_pos();
            if ((*m_read_buffer).enable_read() < ph->size + package_head_len)
               {
                   break;
               }
               uint64_t has_read_len = ph->size + package_head_len;
               if(ph->type == otherpackage){
                   bool flag = m_notify->OnRecvData(m_sessionId,(*m_read_buffer).read_pos() + package_head_len,ph->size);
                   if(flag != true){
                       TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_ON_RECV_DATA_HANDLE_ERROR,"OnRecvData() error,clientid:%d,len:%d,connection will be closed",
                       m_sessionId,ph->size);
                       stop("OnRecvData() error");
                       break;
                   }
                   
               }else{
                   if(heartbeat_client == ph->type){
                        TRACE_INFO(BASE_TAG,"recv client heartbeat");
                        deliver(*ph,nullptr);
                    }else{
                        TRACE_INFO(BASE_TAG,"recv server heartbeat,ph->type:%d",ph->type);
                    }
                    (*m_heartbeat_handle).cancel();
               }
                (*m_read_buffer).has_read(has_read_len);
            }
          do_read();
        }else{
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_READ_SOME,"async_read_some() error,err code:%d,err msg:%s",err.value(),err.message().c_str());
            stop("async_read_some() error," + err.message());
        }
    }

    void session::do_write(){
       if(m_is_connected.load() ){
            int16 oval = 0,nval = 1;
            if(m_send_cell_is_sending.compare_exchange_strong(oval,nval)){
                if(1 == (*m_write_cell).enable_read()){
                    //TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_READ_SOME,"async_write_some");
                    (*m_sock).async_write_some(boost::asio::buffer((*m_write_cell).read_chche(),(*m_write_cell).read_enable_len()),
                    bind(&session::handle_write,this,_1,_2));
                }else{
                    TRACE_INFO(BASE_TAG,"send cache is empty,send heart beat");
                    m_send_cell_is_sending.store(0);
                    (*m_heartbeat_send).expires_from_now(boost::posix_time::milliseconds(heartBeatTime));
                    (*m_heartbeat_send).async_wait(bind(&session::heart_beat_send,this,_1));
                }   
            }else{
                TRACE_INFO(BASE_TAG,"client is sending");
            }
        } 
    }
    void session::handle_write(const boost::system::error_code & err, size_t bytes){
        if(!err){
            (*m_write_cell).has_read(bytes);
            m_send_cell_is_sending.store(0);
            do_write();
        }else{
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_SOME,"async_write_some() error,err code:%d,err msg:%s",err.value(),err.message().c_str());
            stop("async_read_somer() error," + err.message());
        }
    }

    void session::heart_beat_send(const boost::system::error_code & err)
    {
        if (err != boost::asio::error::operation_aborted)
        {
            deliver(m_heartbeat_send_ph,nullptr);
            (*m_heartbeat_handle).expires_from_now(boost::posix_time::milliseconds(heartBeatTime));
            (*m_heartbeat_handle).async_wait(bind(&session::heart_beat_handle,this,_1));
        }else{
            TRACE_WARNING(BASE_TAG,"heart_beat_send cancel");
        }
        
    }

    int session::deliver(package_head& ph,const char*ptr)
    {
        if(m_is_connected.load()){
            int ret = 0 ;
            for(int32_t i = 0;i < 3;i++){
                ret = (*m_write_cell).write_chche(ph,ptr);
                if(CROSS_PLATFORM_AT_S_OK == ret ){
                    break;
                }else{
                    std::this_thread::sleep_for(std::chrono::milliseconds(1 + i));
                }
            }
            // auto ret = (*m_write_cell).write_chche(ph,ptr);
            if(CROSS_PLATFORM_AT_S_OK != ret){
                TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_CACHE_FULL,"write_chche() error,clientid:%d,error code:%d",m_sessionId,ret);
                //stop("send cache is full,connection is closed,sessionid:" + m_sessionId);
                return ret;
            }
            
            do_write();  
            return ret;
        }else{
            return CROSS_PLATFORM_AT_E_TCP_SERVER_CONNECTION_HAS_BEEN_DISCONNECTED;
        }
        
    }

    int session::send_data(const char* buf,const uint64_t len)
    {
        package_head ph;
        ph.size = len;
        ph.type = otherpackage;
        deliver(ph,buf);
    }

    void session::stop(string msg)
    {

        m_ios.post(bind(&session::handle_close,this));
    }

    void session::handle_close()
    {
        if(m_is_connected.load()){
            m_is_connected.store(0);
            TRACE_INFO(BASE_TAG,"connection will be closed,sessionid:%d",m_sessionId);
            
            boost::system::error_code ec;
            (*m_sock).shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
            TRACE_INFO(BASE_TAG,"shutdown() msg,err code:%d,err msg:%s",ec.value(),ec.message().c_str());
            (*m_sock).close(ec);
            TRACE_INFO(BASE_TAG,"close() msg,err code:%d,err msg:%s",ec.value(),ec.message().c_str());

            (*m_heartbeat_send).cancel();
            (*m_heartbeat_handle).cancel();
            m_notify->OnDisconnect(m_sessionId);
        }
    }

    void session::heart_beat_handle(const boost::system::error_code & err)
    {
        if (err != boost::asio::error::operation_aborted)
        {
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_HEARTBEAT_OUT_OF_TIME,"heartbeat out of time,connection will be closed,seddionid:%d,errcode:%d",
                m_sessionId,err.value());
            stop("heartbeat out of time");
        }
    }
}