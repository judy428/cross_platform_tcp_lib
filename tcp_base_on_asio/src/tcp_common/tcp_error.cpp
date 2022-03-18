#include "tcp_error.h"

namespace tcpAsio{

string get_error_msg(uint64_t err_code)
    {
        string errmsg;
        switch (err_code)
        {
        case CROSS_PLATFORM_AT_S_OK:
            errmsg = "ok ";
            break;
        case CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_CACHE_FULL:
            errmsg = "send cache is full,connection is closed";
            break;
        default:
            break;
        }
        return errmsg;
    }


}