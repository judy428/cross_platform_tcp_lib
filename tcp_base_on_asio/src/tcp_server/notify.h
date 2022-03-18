/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: notify.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/
#ifndef _NOTIFY_H_
#define _NOTIFY_H_


namespace tcpAsio
{
class tNotify{
    public: //保证OnConnect，OnRecvData，OnDisconnect调用顺序，都在同一个EventLoop线程
    virtual void OnConnect(const unsigned int clientid, const char* ip, const unsigned short port) = 0;
    virtual void OnDisconnect(const unsigned int clientid) = 0;
    virtual bool OnRecvData(const unsigned int clientid, const char* buf, const size_t len) = 0;
    virtual void OnError(const unsigned int clientid, const int errorid) = 0; //(errorid > 0); -2:接受缓冲区已满; -3:发送接收缓存已满; 
};

}

#endif