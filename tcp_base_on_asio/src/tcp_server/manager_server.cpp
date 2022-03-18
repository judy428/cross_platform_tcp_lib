#include <chrono>
#include <iostream>
#include "manager_server.h"
#include "../tools.h"

using namespace std;
namespace tcpAsio{

managerServer::managerServer(tNotify* tn,int32_t mid,int cpuN):m_TNotify(tn),m_manager_id(mid),m_bind_cpu(cpuN){
    
}


managerServer::~managerServer()
{
    stop();
}

void managerServer::start()
{
    m_thread = make_shared<std::thread>(bind(&managerServer::run,this));
}

void managerServer::stop()
{
    
    if(!m_manager_service.stopped()){
        TRACE_INFO(BASE_TAG,"manager will be stoped,manager id:%d",m_manager_id);
        for(auto &it : m_session)
        {
            (*(it.second)).stop("server closed");
        }
        m_session.clear();
        m_manager_service.stop();
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

void managerServer::run()
{
    // //bind cpu
    // if(bind_cpu(curr_thread_id(),m_bind_cpu) != 0)
    // {
    //     TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_BIND_CPU,"listen epoll bind cpu failed,thread_id:%d",(*m_thread).get_id());
    // }

    m_work = make_shared<io_service::work>(m_manager_service);
    m_time_checker = make_shared<deadline_timer>(m_manager_service,boost::posix_time::milliseconds(timeCheck));
    (*m_time_checker).async_wait(bind(&managerServer::check_session,this));
    m_manager_service.run();
    
}

void managerServer::check_session()
{
    for(auto it = m_session.begin();it != m_session.end();)
    {
         if((*(it->second)).is_running()) 
        {
            it++;
        }else{
            TRACE_INFO(BASE_TAG,"connection will be delete from manager,clientid:%d",it->first);
            m_session.erase(it++);
        }
    }

    (*m_time_checker).expires_from_now(boost::posix_time::milliseconds(timeCheck));
    (*m_time_checker).async_wait(bind(&managerServer::check_session,this));
}


int managerServer::send_data(const uint64_t clientid,const char* buf, const uint64_t len)
{
    if(m_session.find(clientid) != m_session.end()){
        return (*m_session[clientid]).send_data(buf,len);
    }else{
        return CROSS_PLATFORM_AT_E_TCP_SERVER_NOT_FIND_SESSION;
    }
}


void managerServer::add_new_session(const sock_ptr sock,const uint64_t clientid)
{
    m_session[clientid] = make_shared<session>(sock,m_manager_service,clientid,m_TNotify);
    (*m_session[clientid]).start();
  
}

}