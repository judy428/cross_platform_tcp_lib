#ifdef _MSC_VER
#define COMMON_EXPORT
#else
#endif
#include "../../src/tcp_client/client.h"
#include "../../../base/src/trace.h"
#include "../../../base/src/message_base.h"
#include "../tcp_struct.h"
#include "../structInfo.h"

using namespace tcpAsio;
using namespace base;
atomic<int> m_num;
const uint32 conn_num = 40;
uint32 msg_num = 20000;
class tnotify
:public tCliNotify{
public:
    virtual void OnConnect()
    {
        TRACE_INFO(0,"connection");
    }
    virtual void OnDisconnect()
    {
        TRACE_INFO(0,"disConnection");
    }
    virtual bool OnRecvData( const char* ptr, const size_t len)
    {
        if(ptr == nullptr || len <= 0){
            TRACE_ERROR("client_test",2,"recv data param error,msg len:%d",len);
            return false;
        }

        if(len < msg_header_len){
            TRACE_ERROR("client_test",2,"recv errmsg,len < msg_header_len,clientid");
            return 1;
        }
        msg_header* m_header = (msg_header*)ptr;
        if(m_header->data_size != len - msg_header_len){
            TRACE_ERROR("client_test",2,"recv errmsg,data_size != len - msg_header_len");
            return 2;
        }
        atp_message msg;
        msg.type = m_header->type;
        //cout<<"msg.type:"<<msg.type<<endl;
        // cout<<ptr + msg_header_len<<endl;
        // msg.param1 = new char[m_header->data_size + 1];
        // memset(msg.param1,0,m_header->data_size + 1);
        // memcpy(msg.param1,ptr + msg_header_len,m_header->data_size);
        // handle_msg(msg);
        m_num++;
        return true;
    }
    

    virtual void handle_msg(atp_message& msg)
    {
        switch (msg.type)
        {
        case CMD_LOGIN_RESULT:
        {
            LoginResult *n = (LoginResult *)(msg.param1);
            //TRACE_SYSTEM("test","收到服务端消息:CMD_LOGIN_RESULT,n->result:%d,n->data:%s",n->result,n->data);
        }
        break;
        case CMD_LOGOUT_RESULT:
        {
            LogoutResult *n = (LogoutResult *)(msg.param1);
            TRACE_SYSTEM("test","收到服务端消息:CMD_LOGOUT_RESULT,result:%d", n->result);
        }
        break;
        case CMD_NEWUSR_JOIN:
        {
            NewUsrJoin *n = (NewUsrJoin *)(msg.param1);
            TRACE_SYSTEM("test","收到服务端消息:新用户加入,sockid:%d", n->sockid);
        }
        break;

        default:
            TRACE_SYSTEM("test","收到未定义消息 ");
        break;
        }
        return ;
    }
    
};

void wp(client* ct)
{
    Login login;
    strcpy(login.usrName, "lsm");
    strcpy(login.data, "hello world");
    strcpy(login.passWd, "154029");
    msg_header m_header;
    m_header.type = CMD_LOGIN;
    m_header.data_size = sizeof(login);
    int msg_len = sizeof(m_header) +  sizeof(login);
        
    int total_len = sizeof(m_header) + sizeof(login) + 1;
    char* buf = new char[total_len];
    memcpy(buf ,&m_header,sizeof(m_header));
    memcpy(buf + sizeof(m_header),&login,sizeof(login));
    
    //cout<<"m_header.type:"<<m_header.type<<",len:"<<m_header.data_size<<":total_len:"<<total_len<<endl;
    for(int i=0;i<msg_num;i++){
        int ret = ct->send_data(buf,total_len - 1);
        if(ret !=0){
            cout<<"send data error,errcode:"<<ret<<endl;
            break;
        }
    }
}

int test()
{
    m_num = 0;
    tnotify tf;

    shared_ptr<client> ct = make_shared<client>("127.0.0.1",3319,&tf);
    
    if(ct->start() != true){
        TRACE_ERROR("client_test",3,"start() error");
        return 0;
    }
    TRACE_ERROR("client_test",3,"****************************begin");
    thread th(wp,ct.get());
    th.join();
    //wp(ct.get());
    TRACE_ERROR("client_test",3,"****************************end");
    std::this_thread::sleep_for(std::chrono::seconds(80));
    cout<<"m_num:"<<m_num<<endl;
    return 0;
}

int stress_test()
{
    m_num = 0;
    tnotify tf;

    shared_ptr<client> cs[conn_num];
    thread* th[conn_num];
    int errNum = 0;
    for(int i = 0;i < conn_num;i++)
    {
        cs[i] = make_shared<client>("127.0.0.1",3319,&tf);
        auto flag = (*cs[i]).start();
        if(flag == false){
            cout<<"client start error"<<endl;
            errNum++;
        }
    }
    cout<<"errNum:"<<errNum<<endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    TRACE_ERROR("client",1,"send begin********************************************************");
    for(int i = 0;i < conn_num;i++)
    {
        th[i] = new thread(wp,cs[i].get());
    }

    for(int i = 0;i < conn_num;i++)
    {
        th[i]->join();
    }
    TRACE_ERROR("client",2,"send finished******************************************************");
    while(m_num < conn_num * msg_num){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        //TRACE_ERROR("test",3,"m_num:%d",m_num.load());
    }

    TRACE_ERROR("test",3,"cost time:%d",std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTime).count());

    TRACE_ERROR("test",3,"recv num:%d",m_num.load());
    cout<<"m_num:"<<m_num<<endl;
    cout<<"finished"<<endl;
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}

int main()
{
    trace::set_trace_level_filter(TRACE_LEVEL_ERROR);
    return stress_test();
}