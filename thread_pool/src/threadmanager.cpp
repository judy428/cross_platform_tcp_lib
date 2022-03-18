/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: threadmanager.cpp
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#include "threadmanager.h"

namespace threadpool
{
thread_manager::thread_manager()
{
	m_thread_pool_ = new thread_pool();
}

thread_manager::thread_manager(size_t num_)
{
	m_thread_pool_ = new thread_pool(num_);
}

thread_manager::~thread_manager()
{
	if(m_thread_pool_)
	{
		delete m_thread_pool_;
		m_thread_pool_ = nullptr;
	}
}

bool thread_manager::Run(job* job_,void* param_)
{
	return m_thread_pool_->Run(job_,param_);
}

thread_manager_async::thread_manager_async()
{
	m_thread_pool_async_ = new thread_pool_async();
	m_thread_pool_async_->start();
}

thread_manager_async::thread_manager_async(size_t num_)
{
	m_thread_pool_async_ = new thread_pool_async(num_);
	m_thread_pool_async_->start();
}

thread_manager_async::~thread_manager_async()
{
	if(m_thread_pool_async_)
	{
		delete m_thread_pool_async_;
		m_thread_pool_async_ = nullptr;
	}
}

bool thread_manager_async::Run(job* job_,void* param_)
{
	return m_thread_pool_async_->Run(job_,param_);
}
}



