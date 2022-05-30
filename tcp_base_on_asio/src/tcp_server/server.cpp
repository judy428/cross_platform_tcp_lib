#include <functional>
#include <algorithm>
#include <thread>
#include "server.h"
#include "../tools.h"

using namespace std::placeholders;
namespace tcpAsio
{
    tcpServer::tcpServer(const ip::tcp::endpoint& endpoint,tNotify* tn,size_t handleNum):m_acceptor(m_accept_service,endpoint),
        m_base_sessionid(10),m_TNotify(tn),m_handle_num(handleNum)
    {
        // if(m_handle_num < boost::thread::hardware_concurrency() ){
        //     m_handle_num = boost::thread::hardware_concurrency() * 3;
        // }
        if(m_handle_num < std::thread::hardware_concurrency() ){
            m_handle_num = std::thread::hardware_concurrency() * 3;
        }
		

        int cpuN = std::thread::hardware_concurrency() ;
        int currCpu = 0;
        m_bind_cpu = currCpu++ % cpuN;
        // TRACE_INFO(BASE_TAG,"m_handle_num:%d",m_handle_num);
        for (size_t i = 0; i < m_handle_num; i++)
        {
            m_manager_service.push_back(make_shared<managerServer>(m_TNotify,i,currCpu++ % cpuN));
        }
        
    }
    tcpServer::~tcpServer()
    {
        stop();
    }
    void tcpServer::start(){
        
        for (auto& it :m_manager_service)
        {
            it->start();
        }
        m_thread = make_shared<std::thread>(&tcpServer::run,this);
        //run();
    }

    void tcpServer::run(){
        // //bind cpu
        // //if(bind_cpu(std::hash<std::thread::id>{}(std::this_thread::get_id()),m_bind_cpu) != 0)
        // if(bind_cpu(curr_thread_id(),m_bind_cpu) != 0)
        // {
        //     TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_BIND_CPU,"listen epoll bind cpu failed,thread_id:%d",(*m_thread).get_id());
        // }

        do_connect();
        m_accept_service.run();
    }

    shared_ptr<managerServer>& tcpServer::get_manager_server()
    {
         auto it = std::min_element(m_manager_service.begin(),m_manager_service.end(),[](const shared_ptr<managerServer>& p1,const shared_ptr<managerServer>& p2){
            return p1->get_session_num() > p2->get_session_num();});
        return *it;
    }

    
    void tcpServer::handle_connect(sock_ptr sock,managerServer_ptr ms,uint64_t clientid,const boost::system::error_code & er)
    {
        if(!er){
            TRACE_INFO(BASE_TAG,"recv new connection,ip:%s,port:%d",(*sock).remote_endpoint().address().to_string().c_str(),(*sock).remote_endpoint().port());
            (*ms).add_new_session(sock,clientid);
        }else{
            TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_ACCEPT_ERROR,"async_accept() error,accept new connect error,errcode:%d,errmsg:%s",er.value(),er.message().c_str());
        }
        do_connect();
    }

    void tcpServer::do_connect()
    {
        auto clientid = m_base_sessionid++;
        managerServer_ptr ms = CurrCellManager(clientid);
        sock_ptr sock = make_shared<ip::tcp::socket>((*ms).get_io_server());
        m_acceptor.async_accept(*sock,std::bind(&tcpServer::handle_connect,this,sock,ms,clientid,std::placeholders::_1));
       
    }

    int32_t tcpServer::SendData(const int clientid,const char* buf, const uint64_t len)
    {
        managerServer_ptr ms = CurrCellManager(clientid);
        auto ret = (*ms).send_data(clientid,buf,len);
        if(CROSS_PLATFORM_AT_S_OK != ret){
            TRACE_ERROR(BASE_TAG,ret,"send_data error,clientid:%d,error code:%d",clientid,ret);
        }
        return ret;
    }

    void tcpServer::stop()
    {
        if(!m_accept_service.stopped()){
            m_accept_service.stop();
            for(auto &it : m_manager_service)
            {
                (*it).stop();
            }
            m_manager_service.clear();
        }
        if(m_thread.get() != nullptr){
            if((*m_thread).joinable()){
                (*m_thread).join();
                m_thread.reset();
            }else{
                (*m_thread).detach();
            }
        }
    }
}