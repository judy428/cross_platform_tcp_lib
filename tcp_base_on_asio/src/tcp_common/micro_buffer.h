/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: buffer.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/


#ifndef _MICRO_BUFFER_H_
#define _MICRO_BUFFER_H_

#include <iostream>
#include "common.h"


namespace tcpAsio
{
/*
note single thread read and single thread write
*/
class micro_buffer{
public:
    micro_buffer(uint64_t size = readBufSize);
    ~micro_buffer();
    void clear();
    char* write_pos(){return buf + m_write_pos;}
    uint64_t enable_write(){return m_total_size  - m_write_pos;}
    void has_write(uint64_t len){m_write_pos += len;}

    uint64_t enable_read(){ return m_write_pos - m_read_pos;}
    char* read_pos(){return buf + m_read_pos;}
    void has_read(uint64_t len);

private:
    uint64_t m_read_pos;
    uint64_t m_write_pos;
    uint64_t m_total_size;
    char* buf;
};

}

#endif