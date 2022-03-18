#include "micro_buffer.h"

namespace tcpAsio
{
micro_buffer::micro_buffer(uint64_t len ):m_total_size(len)
{
    buf = new char[m_total_size] ;
    memset(buf,0,m_total_size);
    m_write_pos = 0;
    m_read_pos = 0;
}

 micro_buffer::~micro_buffer()
 {
     clear();
 }

void micro_buffer::clear(){
    if(buf != nullptr){
         delete[] buf;
         buf = nullptr;
     }
}
void micro_buffer::has_read(uint64_t len )
{
    m_read_pos += len;
    if(m_read_pos >= m_total_size/2){
        memcpy(buf,buf + m_read_pos,m_write_pos - m_read_pos);
        //strncpy(buf,buf + m_read_pos,m_write_pos - m_read_pos);
        m_write_pos -= m_read_pos;
        m_read_pos = 0;
    }
}

}