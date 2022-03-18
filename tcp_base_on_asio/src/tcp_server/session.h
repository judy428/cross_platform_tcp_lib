/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: session.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/

#ifndef _SESSION_H_
#define _SESSION_H_

#include <iostream>
#include <memory>
#include <vector>
#include <atomic>
#include <string>
#include "boost/asio.hpp"
#include "notify.h"
#include "../tcp_common/micro_buffer.h"
#include "../tcp_common/send_cell.h"
#include "../tcp_common/common.h"

using namespace boost::asio;
using namespace std;
namespace tcpAsio
{

class session{
public:
    session(sock_ptr sock,io_service& ser,uint64_t sessionId,tNotify* tn);
    ~session();
    void start();
    void stop(string);
    int send_data(const char* buf,const uint64_t len);
    int64_t is_running(){return m_is_connected.load();}
private:
    void do_read();
    void handle_read(const boost::system::error_code & err, size_t bytes);

    void do_write();
    void handle_write(const boost::system::error_code & err, size_t bytes);
    void handle_close();
    void heart_beat_send(const boost::system::error_code & err);
    void heart_beat_handle(const boost::system::error_code & err);
    
    int deliver(package_head& ph,const char*ptr);

private:
    shared_ptr<micro_buffer> m_read_buffer;
    shared_ptr<send_cell> m_write_cell;
    sock_ptr m_sock;
    io_service& m_ios;
    deadline_ptr m_heartbeat_send;
    deadline_ptr m_heartbeat_handle;
    uint64_t m_sessionId;
    atomic<int64_t> m_is_connected;
    tNotify* m_notify;
    package_head m_heartbeat_send_ph;
    atomic<int16> m_send_cell_is_sending;
};

}

#endif