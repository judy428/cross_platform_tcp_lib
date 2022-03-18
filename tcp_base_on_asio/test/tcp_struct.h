/*****************************************************************************
 common Module Copyright (c) 2017. All Rights Reserved.

 FileName: tcp_server.h
 Version: 1.0
 Date: 2017.04.10

 History:
 zhujy     2017.04.27   1.0     Create
 ******************************************************************************/

#ifndef TCP_STRUCT_H_
#define TCP_STRUCT_H_

    typedef struct msg_header
    {
        int type;
        int data_size;
        msg_header()
        :type(0),
        data_size(0)
        {}
    }MSG_HEADER;

    struct buf_info
    {
        char* pstr;
        int len;
        buf_info() :
            pstr(NULL),
            len(0){}
    };
    const int msg_header_len = sizeof(msg_header);
    
#endif