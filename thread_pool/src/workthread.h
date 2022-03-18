/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: workthread.h
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_
#include "base/thread.h"
#include "job.h"
#include <condition_variable>
#include <mutex>

using namespace base;
using namespace std;
namespace threadpool
{
class work_thread:public process_thread
{
public:
	work_thread();
	~work_thread();
public:
	virtual void run() override;

		virtual void start() override;
		virtual void stop() override;
public:
		bool set_job(job* job_,void* param_);
		bool get_status()
		{
			return is_idle_;
		}
private:
	job* m_job_;
	void* m_data_;
	bool is_idle_;
	std::mutex m_mutex_;
	std::condition_variable m_condition_var_;
};

}

#endif /* WORKTHREAD_H_ */
