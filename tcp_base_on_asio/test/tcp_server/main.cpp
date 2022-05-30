
 #ifdef _MSC_VER
#define COMMON_EXPORT
#else
#endif
#include <thread>
#include <chrono>
#include <iomanip> 
#include <stdlib.h>
#include "../../src/tcp_server/server.h"
#include "message_center.h"
#include "../structInfo.h"
#include "../tcp_struct.h"
#include "../../../base/src/trace.h"
#include "../../../base/src/mqueue.h"

using namespace tcpAsio;
using namespace base;
const int MAXSENDBUF = 1024 *4;
int main(){
    trace::set_trace_level_filter(TRACE_LEVEL_ERROR);
    srt_queue<atp_message> m_queue;
    m_queue.init();

    message_center m_msg(&m_queue);
    
   ip::tcp::endpoint ep( ip::tcp::v4(), 3319);
    shared_ptr<tcpServer> m_tcp_server_ = make_shared<tcpServer>(ep,&m_msg);
    (*m_tcp_server_).start();
    //sleep(10 * 1);
    
    msg_header m_header;
    char* ptr =  new char[MAXSENDBUF];
    char data[100];
    int64_t num = 0;
    cout<<"-------------------------------size:"<<m_queue.size()<<endl;
    atp_message msg;
    //sleep(1);
    
    while(1){
        if(m_queue.size() == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }
        if(!m_queue.pop(msg)){
            continue;
        }
        
        if(msg.param1 != nullptr){
            switch (msg.type)
            {
            case CMD_LOGIN:
            {
                //delete[] (msg.param1);
                Login* lg = (Login *)(msg.param1);
                //delete lg;
                // TRACE_SYSTEM("test","recv msg,lg->usrName:%s,lg->passWd:%s",lg->usrName,lg->passWd);
                
                //char data[100];
                sprintf(data,"wsc:%d",num++);
                LoginResult* lr = new LoginResult();
                lr->result = 2;
                strcpy(lr->data,data);
                
                m_header.data_size = sizeof(LoginResult);
                m_header.type = CMD_LOGIN_RESULT;
                memset(ptr,0,sizeof(m_header) +  sizeof(LoginResult) + 1);
                memcpy(ptr,&m_header,sizeof(m_header));
                memcpy(ptr + sizeof(m_header),lr,sizeof(LoginResult));
                
                m_tcp_server_->SendData(*((int *)(msg.param2) ),ptr,sizeof(m_header) +  sizeof(LoginResult));
                delete lr;
                lr = nullptr;
                
            }
            break;
            default:
            TRACE_ERROR("server",1,"msg.type error,msg.type:%d",msg.type);
                break;
            }

            //释放内存
            //cout<<"address:"<<msg.param1<<endl;
            // delete[] msg.param1;
            delete[] (msg.param1);
            // delete[] msg.param1;
            delete (int *)(msg.param2);
            msg.param1 = nullptr;
            msg.param2 = nullptr;
        }
        
    }

    cout<<"num:"<<num<<endl;
     delete[] ptr;
    
    cout<<"1111111111"<<endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
	(*m_tcp_server_).stop();
    
    cout<<"finished"<<endl;
    
    return 0;
}