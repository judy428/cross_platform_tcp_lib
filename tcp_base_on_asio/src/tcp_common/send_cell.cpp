#include "send_cell.h"


namespace tcpAsio
{
send_cell::send_cell()
{
    m_read_pos = 0;
    m_write_pos = 0;
}

void send_cell::clear()
{
    TRACE_INFO(BASE_TAG,"clear send_cell,m_write_pos:%d,m_read_pos:%d",m_write_pos.load(),m_read_pos.load());
    while (!is_empty())
    {
        uint64_t rpos = m_read_pos.load() % sendCellSize;
        if(m_scu_ptr[rpos] != nullptr){
            delete[] m_scu_ptr[rpos]->ptr;
            m_scu_ptr[rpos]->ptr = nullptr;
            delete m_scu_ptr[rpos];
            m_scu_ptr[rpos] = nullptr;
            m_read_pos++;
        }
    }
}
send_cell::~send_cell( )
{
    clear();
}

int send_cell::write_chche(package_head& ph,const char* ptr)
{
    if (is_full())
    {
         TRACE_ERROR(BASE_TAG,CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_CACHE_FULL,"send cache is ful,m_read_pos:%d,m_write_pos:%d",m_read_pos.load(),m_write_pos.load());
         return CROSS_PLATFORM_AT_E_TCP_SERVER_WRITE_CACHE_FULL;
    }
    send_cell_unit* sc = new send_cell_unit();
    assert(sc != nullptr);
    sc->bufSize = (int((package_head_len + ph.size)/sendCellUnitSingleSize + 1)) * sendCellUnitSingleSize;
    sc->ptr = new char[ sc->bufSize];
    assert(sc->ptr != nullptr);

    memcpy(sc->ptr,&ph,package_head_len);
    memcpy(sc->ptr + package_head_len,ptr, ph.size);
    sc->sendPos = 0;
    sc->size = package_head_len + ph.size;
    auto wpos = m_write_pos.fetch_add(1) %sendCellSize;
    m_scu_ptr[wpos] = sc;
    m_scu_ptr[wpos]->enableRead = 1;
    return CROSS_PLATFORM_AT_S_OK;
}

int send_cell::is_empty()
{
    return (m_write_pos.load() % sendCellSize == m_read_pos.load() % sendCellSize)?1:0;
}


int send_cell::is_full()
{
    return (((m_write_pos.load() + 1) % sendCellSize) == m_read_pos.load() % sendCellSize)?1:0;
}

int send_cell::enable_read()
{
    if(m_write_pos.load() % sendCellSize == m_read_pos.load() % sendCellSize){
        return 0;
    }else{
        uint64_t rpos = m_read_pos.load() % sendCellSize;
        if(m_scu_ptr[rpos] == nullptr){
            return 0;
        }
        if(m_scu_ptr[rpos]->enableRead == 0){
            return 0;
        }
    }
    return 1;
}

char* send_cell::read_chche()
{
    if(0 == enable_read())
    {
        return nullptr;
    }else{
        uint64_t rpos = m_read_pos.load() % sendCellSize;
        return m_scu_ptr[rpos]->ptr + m_scu_ptr[rpos]->sendPos;
    }
}

uint64_t send_cell::read_enable_len()
{
    if(0 == enable_read())
    {
        return 0;
    }else{
        uint64_t rpos = m_read_pos.load() % sendCellSize;
        return m_scu_ptr[rpos]->size - m_scu_ptr[rpos]->sendPos;
    }
}


void send_cell::has_read(size_t len)
{
    uint64_t rpos = m_read_pos.load() % sendCellSize;
    m_scu_ptr[rpos]->sendPos += len;
    if (m_scu_ptr[rpos]->size <= m_scu_ptr[rpos]->sendPos )
    {
        m_scu_ptr[rpos]->enableRead = 0;
        m_read_pos.fetch_add(1);
        delete[] m_scu_ptr[rpos]->ptr;
        m_scu_ptr[rpos]->ptr = nullptr;
        delete m_scu_ptr[rpos];
        m_scu_ptr[rpos] = nullptr;
    }
}

}