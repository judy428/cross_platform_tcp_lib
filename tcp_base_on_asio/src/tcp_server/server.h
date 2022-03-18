/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: server.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/
#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <boost/asio.hpp>
#include "manager_server.h"
#include "../tcp_common/common.h"
#include "notify.h"


using namespace std;
using namespace boost::asio;

namespace tcpAsio
{

class tcpServer{
public:
    //
    //ip,回调类,实际处理线程数（默认不要填）
    tcpServer(const ip::tcp::endpoint& endpoint,tNotify* tn,size_t handleNum = 0);
    tcpServer(const tcpServer&) = delete;
    tcpServer operator = (const tcpServer&) = delete;
    ~tcpServer();
public:
    
    void start();
    void stop();
    void CloseClient( int clientid);
    int32_t SendData(const int clientid,const char* buf, const uint64_t len);  
private:
    typedef shared_ptr<session> session_ptr;
    typedef shared_ptr<micro_buffer> buffer_ptr;
    typedef shared_ptr<managerServer> managerServer_ptr;
    void run();
    void do_connect();
    void handle_connect(sock_ptr sock,managerServer_ptr ms,uint64_t clientid,const boost::system::error_code & err);
    //void handle_connect1(ip::tcp::socket sock,managerServer_ptr ms,uint64_t clientid,const boost::system::error_code & err);
    
    shared_ptr<managerServer> CurrCellManager( int clientid){return m_manager_service[clientid%m_manager_service.size()];};
    shared_ptr<managerServer>& get_manager_server();
private:
    io_service m_accept_service;
    ip::tcp::acceptor m_acceptor;
    vector<shared_ptr<managerServer> > m_manager_service;
    int m_handle_num;
    atomic<uint64_t> m_base_sessionid;
    tNotify* m_TNotify;
    thread_ptr m_thread;
    int m_bind_cpu;
};

}

#endif