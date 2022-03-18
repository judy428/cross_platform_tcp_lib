/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: threadmanager.h
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include "threadpool.h"
#include <unistd.h>

namespace threadpool
{
class thread_manager
{
public:
	thread_manager();
	thread_manager(size_t num_);
	~thread_manager();
public:
	bool Run(job* job_,void* param_);
private:
	thread_pool* m_thread_pool_;
};

class thread_manager_async
{
public:
	thread_manager_async();
	thread_manager_async(size_t num_);
	~thread_manager_async();
public:
	bool Run(job* job_,void* param_);
private:
	thread_pool_async* m_thread_pool_async_;
};
}


#endif /* THREADMANAGER_H_ */
