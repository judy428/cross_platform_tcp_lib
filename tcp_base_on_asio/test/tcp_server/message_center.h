
#ifndef MESSAGE_CENTER_H
#define MESSAGE_CENTER_H

#include <thread>
#include <chrono>
#include <iostream>
#include "../../../base/src/message_base.h"
#include "../../../base/src/thread.h"
#include "../../../base/src/mqueue.h"
#include "../../src/tcp_server/notify.h"

using namespace std;
using namespace tcpAsio;
using namespace base;
class message_center 
:public message_dispatcher
,public tNotify
{
public:
    message_center(srt_queue<atp_message>* mq);
public:
    virtual void OnConnect(const unsigned int clientid, const char* ip, const unsigned short port);
	virtual void OnDisconnect(const unsigned int clientid);
	virtual bool OnRecvData(const unsigned int clientid, const char* buf, const size_t len);
	virtual void OnError(const unsigned int clientid, const int errorid);
public:
    virtual int dispatch_message(atp_message& msg);
    virtual int64_t on_read(const int clientid,const char* ptr,const int len);
    
private:
        //lockFreeQueue<atp_message>* m_queue;
        srt_queue<atp_message>* m_queue;
        atomic<int64_t> m_total;
};


#endif