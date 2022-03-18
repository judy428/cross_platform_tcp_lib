#include <functional>
#include <chrono>
#include "client.h"

using namespace std::placeholders;
using namespace boost::asio;

namespace tcpAsio
{
    client::client(string ip,int64_t port,tCliNotify* tcn):m_tcn(tcn){
        m_is_connected.store(0);
        m_send_cell_is_sending.store(0);
        m_read_buffer = make_shared<micro_buffer>(readBufSize);
        m_write_cell = make_shared<send_cell>();

        m_ep = make_shared<ip::tcp::endpoint>(ip::address::from_string(ip), port);
        m_sock = make_shared<ip::tcp::socket>(m_service);

        m_heartbeat_send = make_shared<deadline_timer>(m_service,boost::posix_time::milliseconds(heartBeatTime));
        m_heartbeat_handle = make_shared<deadline_timer>(m_service,boost::posix_time::milliseconds(heartBeatTime));
        m_heartbeat_send_ph.size = 0;
        m_heartbeat_send_ph.type = heartbeat_client; 
    }

    client::~client()
    {
        handle_close();
        if(m_thread.get()){
            if((*m_thread).joinable()){
                (*m_thread).join();
                m_thread.reset();
            }else{
                (*m_thread).detach();
            }
        }
    }

    bool client::start()
    {
        try
        {
           (*m_sock).connect((*m_ep));
        }
        catch(const boost::system::system_error& err)
        {
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_CLIENT_CONNECT_ERROR,"connect() error,err code:%d,err msg:%s",err.code(),err.what());
            return false;
        }
        m_is_connected.store(1);
        
        //set socket buffer;
        boost::asio::socket_base::receive_buffer_size rsize(tcpRecvBufSize);
        (*m_sock).set_option(rsize);
        boost::asio::socket_base::send_buffer_size ssize(tcpSendBufSize);
        (*m_sock).set_option(ssize);

        deliver(m_heartbeat_send_ph,nullptr);
        (*m_heartbeat_handle).expires_from_now(boost::posix_time::milliseconds(heartBeatTime));
        (*m_heartbeat_handle).async_wait(bind(&client::heart_beat_handle,shared_from_this(),_1));
        do_read();
        m_thread = make_shared<thread>(bind(&client::run,shared_from_this()));
        return true;
    }

    void client::stop(string msg)
    {
        m_service.post(bind(&client::handle_close,shared_from_this()));           
        
    }

    void client::handle_close()
    {
        if(m_is_connected.load()){
            m_is_connected.store(0);
            TRACE_ERROR(BASE_TAG,3,"connection will be closed");
            boost::system::error_code ec;
            (*m_sock).shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
            TRACE_INFO(BASE_TAG,"shutdown() msg,err code:%d,err msg:%s",ec.value(),ec.message().c_str());
            (*m_sock).close(ec);
            TRACE_INFO(BASE_TAG,"close() msg,err code:%d,err msg:%s",ec.value(),ec.message().c_str());
            
            (*m_heartbeat_send).cancel();
            (*m_heartbeat_handle).cancel();
            //(*m_write_cell).clear();
            (*m_read_buffer).clear();
            m_tcn->OnDisconnect();
            m_service.stop();
        }
        
    }

    void client::run()
    {
        m_service.run();
    }

    void client::do_read(){
        if(m_is_connected.load()){
            (*m_sock).async_read_some(boost::asio::buffer((*m_read_buffer).write_pos(),(*m_read_buffer).enable_write()),
                bind(&client::handle_read,shared_from_this(),_1,_2));
        }  
    }
    void client::handle_read(const boost::system::error_code & err, size_t bytes){
        TRACE_DEBUG(BASE_TAG,"recv msg,bytes:%d",bytes);
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
               if(ph->size){
                   bool flag = m_tcn->OnRecvData((*m_read_buffer).read_pos() + package_head_len,ph->size);
                   if(flag != true){
                       TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_ON_RECV_DATA_HANDLE_ERROR,"OnRecvData() error,len:%d,connection will be closed",ph->size);
                       stop("OnRecvData() error");
                       break;
                   }
                   
               }else{
                   if(heartbeat_client == ph->type){
                        TRACE_INFO(BASE_TAG,"recv client heartbeat");
                    }else{
                        TRACE_INFO(BASE_TAG,"recv server heartbeat");
                        deliver(*ph,nullptr);
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

    void client::do_write(){
        if(m_is_connected.load() ){
            int16 oval = 0,nval = 1;
            if(m_send_cell_is_sending.compare_exchange_strong(oval,nval)){
                if(1 == (*m_write_cell).enable_read()){
                    TRACE_INFO(BASE_TAG,"async_write_some");
                    (*m_sock).async_write_some(boost::asio::buffer((*m_write_cell).read_chche(),(*m_write_cell).read_enable_len()),
                    bind(&client::handle_write,shared_from_this(),_1,_2));
                }else{
                    TRACE_INFO(BASE_TAG,"send cache is empty,send heart beat");
                    m_send_cell_is_sending.store(0);
                    (*m_heartbeat_send).expires_from_now(boost::posix_time::milliseconds(heartBeatTime));
                    (*m_heartbeat_send).async_wait(bind(&client::heart_beat_send,shared_from_this(),_1));
                }   
            }else{
                TRACE_INFO(BASE_TAG,"client is sending");
            }
        }
    }
    void client::handle_write(const boost::system::error_code & err, size_t bytes){
        if(!err){
            //TRACE_INFO(BASE_TAG,"do_write() finished,size:%d",bytes);
            (*m_write_cell).has_read(bytes);
            m_send_cell_is_sending.store(0);
            do_write();
           
        }else{
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_SOME,"async_write_some() error,err code:%d,err msg:%s",err.value(),err.message().c_str());
            stop("async_read_some() error," + err.message());
        }
    }

    void client::heart_beat_send(const boost::system::error_code & err)
    {
        if (err != boost::asio::error::operation_aborted)
        {
            deliver(m_heartbeat_send_ph,nullptr);
            (*m_heartbeat_handle).expires_from_now(boost::posix_time::milliseconds(heartBeatTime));
            (*m_heartbeat_handle).async_wait(bind(&client::heart_beat_handle,shared_from_this(),_1));
        }else{
            TRACE_WARNING(BASE_TAG,"heart_beat_send cancel");
        }
    }

    int client::deliver(package_head& ph,const char* ptr)
    {
        int ret = 0 ;
        for(int32_t i = 0;i < 4;i++){
            ret = (*m_write_cell).write_chche(ph,ptr);
            if(CROSS_PLATFORM_AT_S_OK == ret ){
                break;
            }else{
                std::this_thread::sleep_for(std::chrono::milliseconds(1 + i));
            }
        } 
        if(CROSS_PLATFORM_AT_S_OK != ret){
            TRACE_ERROR(BASE_TAG,ret,"write_chche() error,error code:%d",ret);
            //直接关闭连接，还是不关闭，让客户自己处理？暂时先关闭
            //stop("send cache is full,connection is closed" );
            return ret;
        }
        do_write();
        return ret;
    }

    int client::send_data(const char* buf,const uint64_t len)
    {
        if(0 != m_is_connected.load()){
            package_head ph;
            ph.size = len;
            ph.type = otherpackage;
            return deliver(ph,buf);
        }else{
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_CLIENT_DISCONNECTED,"connection has been closed");
            return CROSS_PLATFORM_AT_E_TCP_CLIENT_DISCONNECTED;
        } 
    }

    void client::heart_beat_handle(const boost::system::error_code & err)
    {
        if (err != boost::asio::error::operation_aborted)
        {
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_HEARTBEAT_OUT_OF_TIME,"heartbeat out of time,connection will be closed");
            stop("heartbeat out of time");
        }else{
            TRACE_INFO(BASE_TAG,"heart_beat_handle cancel");
        }
    }
}