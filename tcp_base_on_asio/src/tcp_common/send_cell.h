/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: send_cell.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2021.08.19   1.0     Create
******************************************************************************/

#ifndef _SEND_CELL_H_
#define _SEND_CELL_H_

#include <iostream>
#include <memory>
#include <vector>
#include <atomic>
#include "common.h"


namespace tcpAsio
{
struct send_cell_unit{
    char* ptr;
    int size;
    int sendPos = 0;
    int bufSize = sendCellUnitSize;
    int enableRead = 0;
    send_cell_unit():ptr(nullptr),size(0),sendPos(0),enableRead(0){
    }
};

/*
note multiple thread write and single thread read
*/

class send_cell{

public:
    send_cell();
    ~send_cell();
    

    int write_chche(package_head& ph,const char* buf);
    char* read_chche();
    uint64_t read_enable_len();

    int is_empty();
    int is_full();
    // int enable_read(){ return (m_scu_ptr[m_read_pos.load()%sendCellSize] != nullptr)? 1:0;}
    int enable_read();
    void has_read(size_t len);
private:
    void clear();
private:
    send_cell_unit* m_scu_ptr[sendCellSize];
    atomic<uint64_t> m_read_pos;
    atomic<uint64_t> m_write_pos;
};
}
#endif