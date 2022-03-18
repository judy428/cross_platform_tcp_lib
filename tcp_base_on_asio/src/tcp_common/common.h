/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: buffer.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include "../../../base/src/base.h"
#include "../../../base/src/trace.h"
#include "tcp_error.h"



using namespace std;
using namespace boost::asio;
using namespace base;

namespace tcpAsio
{
    typedef shared_ptr<ip::tcp::socket> sock_ptr;
    typedef shared_ptr<deadline_timer> deadline_ptr;
    typedef shared_ptr<thread>  thread_ptr;
    typedef shared_ptr<ip::tcp::endpoint> endpoint_ptr;

    const int heartBeatTime = 15000;        /*毫秒*/
    const int timeCheck = 500;        /*毫秒*/
    const uint64_t sendCellSize = 1024 * 64;
    const int sendCellUnitSingleSize = 1024;
    const int sendCellUnitSize = sendCellUnitSingleSize * 1;
    const int readBufSize = 1024 * 1024;
    const int writeBufSize = 1024 * 1024;

    const int tcpRecvBufSize = 1024 * 1024 * 2;
    const int tcpSendBufSize = 1024 * 1024;

    enum package_type{
        heartbeat_server = 0,
        heartbeat_client,
        otherpackage
    };
    struct package_head{
        int type;
        //uint64_t ff;
        uint64_t size;
    };
    const int package_head_len = sizeof(package_head);
}


#endif