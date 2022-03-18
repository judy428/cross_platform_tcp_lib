/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: threadpool.h
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "base/thread.h"
#include "workthread.h"
#include <condition_variable>
#include <mutex>
#include <list>
#include <atomic>
#include <unistd.h>
#include <sched.h>
#include <queue>

using namespace base;
using namespace std;

namespace threadpool
{
struct thread_status
{
	work_thread* thread_;
	bool is_idle_;
};
class thread_pool
{
public:
	thread_pool():thread_pool(sysconf(_SC_NPROCESSORS_CONF)){};
	thread_pool(size_t num_);
	~thread_pool();
public:
	bool Run(job* job_,void* param_);
protected:
	void init();
private:
	std::list<work_thread*> m_list_thread_;
	size_t m_thread_max_;
	std::atomic<size_t> m_thread_num_;
	std::mutex m_mutex_;
};

struct atp_message
{
	job* job_;
	void* param_;
};
class thread_pool_async :public process_thread
{
public:
	thread_pool_async():thread_pool_async(sysconf(_SC_NPROCESSORS_CONF)){};
	thread_pool_async(size_t num_);
	~thread_pool_async();
public:
	virtual void run() override;

	virtual void start() override;
	virtual void stop() override;
public:
	bool Run(job* job_,void* param_);
	bool push(struct atp_message& msg);
	bool get(struct atp_message& msg);
	void release_msg();
public:
	work_thread* get_thread();
protected:
	void init();
	void reduce_thread();
private:
	std::list<work_thread*> m_list_thread_;
	size_t m_thread_max_;
	std::atomic<size_t> m_thread_num_;
	std::mutex m_mutex_;

	std::mutex m_mutex_msg_;
	std::condition_variable m_condition_msg_;
	std::queue<struct atp_message> m_queue_msg_;
};
}


#endif /* THREADPOOL_H_ */
