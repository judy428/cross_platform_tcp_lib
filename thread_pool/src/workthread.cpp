/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: workthread.cpp
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#include "workthread.h"
#include <chrono>
#include <thread>


namespace threadpool
{
work_thread::work_thread()
:m_job_(nullptr)
,m_data_(nullptr)
{
	is_idle_ = true;
}

work_thread::~work_thread()
{
	stop();
	is_idle_ = true;
}

void work_thread::start()
{
	process_thread::start();
}

void work_thread::stop()
{
	while(m_job_)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds (200));
	}
	m_condition_var_.notify_one();
	process_thread::stop();

}

void work_thread::run()
{
	while(is_running())
	{
		if(nullptr == m_job_)
		{
			std::unique_lock<std::mutex> lock(m_mutex_);
			m_condition_var_.wait(lock);
		}
		if(nullptr == m_job_)
		{
			continue;
		}

		m_job_->workfun(m_data_);
		m_job_ = nullptr;
		m_data_ = nullptr;
		is_idle_ = true;
	}
}

bool work_thread::set_job(job* job_,void* param_)
{
	if(nullptr == job_)
	{
		return false;
	}
	if(m_job_ != NULL || is_idle_ == false)
	{
		return false;
	}
	m_mutex_.lock();
	is_idle_ = false;
	m_job_ = job_;
	m_data_ = param_;
	m_mutex_.unlock();
	m_condition_var_.notify_one();
	return true;
}


}
