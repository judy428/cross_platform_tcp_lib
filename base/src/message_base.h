/*****************************************************************************
 Base Copyright (c) 2015. All Rights Reserved.

 FileName: message_base.h
 Version: 1.0
 Date: 2015.3.11

 History:
 zhujy     2017-11-29   1.0     Create
 ******************************************************************************/


#ifndef MESSAGE_BASE_H_
#define MESSAGE_BASE_H_

#include "mqueue.h"

namespace base
{
struct atp_message
{
    int type;
    void* param1;
    void* param2;
};

class message_dispatcher
{
public:
    virtual ~message_dispatcher(){};
public:
    virtual int dispatch_message(atp_message& msg) = 0;
};

class processor_base
{
public:
	processor_base()
	{
		msg_queue_ = new base::srt_queue<atp_message>(12);
		msg_queue_->init();
	}

	virtual ~processor_base()
	{
		if(msg_queue_ != NULL)
		{
			delete msg_queue_;
			msg_queue_ = NULL;
		}
	}

	virtual void post(const atp_message& msg)
	{
		msg_queue_->push(msg);
	}

protected:
	virtual int get(atp_message& msg)
	{
		if(msg_queue_->pop(msg))
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

protected:
	base::srt_queue<atp_message>* msg_queue_;
};
}


#endif /* MESSAGE_BASE_H_ */
