#ifdef _MSC_VER
#define COMMON_EXPORT
#else
#endif
#include "message_center.h"
#include "../tcp_struct.h"
#include "../structInfo.h"
#include "../../../base/src/trace.h"

using namespace tcpAsio;
using namespace base;
const char* SERVER_TAG = "server_test";
const char* CLIENT_TAG = "client_test";

message_center::message_center(srt_queue<atp_message>* mq){
    m_total.store(0);
    m_queue = mq ;//new lockFreeQueue<atp_message>(1 << 10, nullptr);
}

int64_t message_center::on_read(const int clientid,const char* ptr,const int len){
    if(len < msg_header_len){
        TRACE_ERROR(SERVER_TAG,2,"recv errmsg,len < msg_header_len,clientid:%d",clientid);
        return 1;
    }
    msg_header* m_header = (msg_header*)ptr;
    if(m_header->data_size != len - msg_header_len){
        TRACE_ERROR(SERVER_TAG,2,"recv errmsg,data_size != len - msg_header_len,clientid:%d",clientid);
        return 2;
    }
    atp_message msg;
    msg.type = m_header->type;
    
    msg.param1 = new char[m_header->data_size + 1];
    memset(msg.param1,0,m_header->data_size + 1);
    memcpy(msg.param1,ptr + msg_header_len,m_header->data_size);
    msg.param2 = new int(clientid);
    return dispatch_message(msg);
}

 int message_center::dispatch_message(atp_message& msg){
    if(m_queue->push(msg)){
        return 0;
    }
    return  -1;
}


void message_center::OnConnect(const unsigned int clientid, const char* ip, const unsigned short port)
{
	TRACE_INFO("server_test","server has connected with client,fd:%d,ip:%s,port:%d",clientid,ip,port);
}

void message_center::OnDisconnect(const unsigned int clientid)
{
	TRACE_INFO("server_test","server has disconnected with client,fd:%d",clientid);
}

bool message_center::OnRecvData(const unsigned int clientid, const char* recvbuf, const size_t len)
{
	if(clientid <= 0 || recvbuf == nullptr || len == 0){
		TRACE_ERROR("server_test",1,"recv data param error,clientid:%d,msg len:%d,m_total:%ld",clientid,len,m_total.load());
		return false;
	}
    m_total.fetch_add(1);
	int64 retCode = on_read(clientid,recvbuf,len);
	if(retCode != 0){
		TRACE_ERROR("server_test",2,"OnRecvData handle error,clientid:%d,msg len:%d",clientid,len);
		return false;
	}
	return true;
}


void message_center::OnError(const unsigned int clientid, const int errorid)
{
	TRACE_ERROR("server_test",3,"recv err msg,errcode:%d",errorid);
}