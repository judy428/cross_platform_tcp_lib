/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: manager_server.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/
#ifndef _MANAGER_SERVER_H_
#define _MANAGER_SERVER_H_

#include <iostream>
#include <memory>
#include <vector>
#include <atomic>
#include <unordered_map>
#include "boost/asio.hpp"
#include "session.h"
#include "../tcp_common/common.h"

using namespace std;
using namespace boost::asio;

namespace tcpAsio
{

class managerServer{
public:
    managerServer(tNotify* tn,int32_t mid,int cpuN = 0);
    ~managerServer();
public:
    void start();
    void stop();
    io_service& get_io_server(){return m_manager_service;}
    int get_session_num(){return m_session_num.load();}
    int send_data(const uint64_t clientid,const char* buf, const uint64_t len);
    void add_new_session(const sock_ptr sock,const uint64_t clientid);
private:
    void check_session();
    void run();
private:
    shared_ptr<io_service::work> m_work;
    io_service m_manager_service;
    unordered_map<uint64_t, shared_ptr<session> > m_session;
    tNotify* m_TNotify;
    atomic<uint64_t> m_session_num;
    deadline_ptr m_time_checker;
    thread_ptr m_thread;
    uint32_t m_manager_id;
    int m_bind_cpu;
};

}

#endif