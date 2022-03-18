/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: client.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/

#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <memory>
#include <vector>
#include <atomic>
#include <string>
#include "boost/asio.hpp"
#include "../tcp_common/micro_buffer.h"
#include "../tcp_common/send_cell.h"
#include "../tcp_common/common.h"

using namespace boost::asio;
using namespace std;
namespace tcpAsio
{

class tCliNotify
{
public:
    virtual void OnConnect() = 0;
	virtual void OnDisconnect() = 0;
	virtual bool OnRecvData( const char* buf, const size_t len) = 0;
};

class client:public std::enable_shared_from_this<client>{
public:
    client(string ip,int64_t port,tCliNotify* tcn);
    ~client();
    bool start();
    void stop(string);
    int send_data(const char* buf,const uint64_t len);
    int64_t is_running(){return m_is_connected.load();}
private:
    void run();
    void do_read();
    void handle_read(const boost::system::error_code & err, size_t bytes);

    void do_write();
    void handle_write(const boost::system::error_code & err, size_t bytes);
    void heart_beat_send(const boost::system::error_code & err);
    void heart_beat_handle(const boost::system::error_code & err);
    void do_connect();
   
    void handle_close();
    
    int deliver(package_head& ph,const char*ptr);

private:
    shared_ptr<micro_buffer> m_read_buffer;
    shared_ptr<send_cell> m_write_cell;
    sock_ptr m_sock;
    io_service m_service;
    deadline_ptr m_heartbeat_send;
    deadline_ptr m_heartbeat_handle;
    endpoint_ptr m_ep;
    tCliNotify* m_tcn;
    atomic<int64_t> m_is_connected;
    package_head m_heartbeat_send_ph;
    thread_ptr m_thread;
    atomic<int16> m_send_cell_is_sending;
};

}

#endif