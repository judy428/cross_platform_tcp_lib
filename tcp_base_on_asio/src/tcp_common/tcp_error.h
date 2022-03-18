#ifndef BASE_ERROR_H
#define BASE_ERROR_H

#include <string>

using namespace std;

namespace tcpAsio
{

enum BASE_ERROR
    {
        CROSS_PLATFORM_AT_S_OK = 0,

        /* 100*** tcp server error*/
        CROSS_PLATFORM_AT_TCP_SERVER = 100000,
            /**1001** tcp服务参数error*/
        CROSS_PLATFORM_AT_E_TCP_SERVER_PORT = 100101,                        /*输入端口错误 */
            /**1002** tcp服务运行error*/
        CROSS_PLATFORM_AT_E_TCP_SERVER_ACCEPT_ERROR = 100201, /*接受新连接失败 */  
        CROSS_PLATFORM_AT_E_TCP_SERVER_BIND_CPU = 100202,                  /*绑核失败 */
                           
           
            /**1003** tcp服务客户管理段error*/
            /**1004** tcp服务客户端error*/
        CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_CACHE_FULL = 100401,                   /*发送缓冲区满 */
        CROSS_PLATFORM_AT_E_TCP_SERVER_READ_SOME = 100402,                   /*读取数据失败 */
        CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_SOME = 100403,                   /*写入数据失败 */
        CROSS_PLATFORM_AT_E_TCP_SERVER_NOT_FIND_SESSION = 100404,           /*未找到客户端会话 */
        CROSS_PLATFORM_AT_E_TCP_SERVER_HEARTBEAT_OUT_OF_TIME = 100405,          /*心跳超时 */
        CROSS_PLATFORM_AT_E_TCP_SERVER_ON_RECV_DATA_HANDLE_ERROR = 100406,  /*接受数据处理失败 */
        CROSS_PLATFORM_AT_E_TCP_SERVER_CONNECTION_HAS_BEEN_DISCONNECTED = 100407,  /*客户端链接已断开 */ 

 
        /* 101*** tcp client error*/
        CROSS_PLATFORM_AT_E_TCP_CLIENT_CONNECT_ERROR = 101000,               /* 客户端连接失败*/
        CROSS_PLATFORM_AT_E_TCP_CLIENT_SEND = 101001,                       /*客户端发送失败 */
        CROSS_PLATFORM_AT_E_TCP_CLIENT_DISCONNECTED = 101002,               /* 客户端连接已断开*/
        CROSS_PLATFORM_AT_E_TCP_CLIENT_DISCONNECTED_ERROR = 101003               /* 客户端断开连接失败*/
    };

    string get_error_msg(uint64_t err_code);

}
#endif