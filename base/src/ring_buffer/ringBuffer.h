#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

namespace base {
template <class T>
class ringBuffer
{
public:
    ringBuffer(unsigned size = 1E5): m_size(size), m_front(0), m_rear(0)
    {
        m_data = new T[size];
    }

    ~ringBuffer()
    {
        delete [] m_data;
    m_data = NULL;
    }

    inline bool isEmpty() const
    {
        return m_front == m_rear;
    }

    inline bool isFull() const
    {
        return m_front == (m_rear + 1) % m_size;
    }

    bool push(const T& value)
    {
        if(isFull())
        {
            return false;
        }
        m_data[m_rear] = value;
        m_rear = (m_rear + 1) % m_size;
    return true;
    }

    bool push(const T* value)
    {
        if(isFull())
        {
            return false;
        }
    m_data[m_rear] = *value;
        m_rear = (m_rear + 1) % m_size;
    return true;
    }

    inline bool pop(T& value)
    {
        if(isEmpty())
        {
            return false;
        }
        value = m_data[m_front];
        m_front = (m_front + 1) % m_size;
        return true;
    }
    inline unsigned int front()const
    {
    return m_front;
    }

    inline unsigned int rear()const
    {
    return m_rear;
    }
    inline unsigned int size()const 
    {
        return m_size;
    }
private:
    unsigned int m_size;// 队列长度
    int m_front;// 队列头部索引
    int m_rear;// 队列尾部索引
    T* m_data;// 数据缓冲区
};
}
#endif