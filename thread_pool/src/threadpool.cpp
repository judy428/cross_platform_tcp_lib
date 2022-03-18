/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: threadpool.cpp
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#include "threadpool.h"
#include <unistd.h>
#include <chrono>
#include <iostream>
using namespace std;

namespace threadpool
{
thread_pool::thread_pool(size_t num_)
{
	m_thread_num_ = num_;
	m_thread_max_ = num_ * 2;
	m_list_thread_.clear();
	init();
}
thread_pool::~thread_pool()
{
	for(auto wkth_:m_list_thread_)
	{
		if(nullptr == wkth_)
			continue;
		while(!wkth_->get_status())
		{
			usleep(10);
		}
		delete wkth_;
		wkth_ = nullptr;
	}
	m_list_thread_.clear();
}

void thread_pool::init()
{
	for(size_t i=0;i< m_thread_num_;++i)
	{
		work_thread* ts_;
		ts_ = new work_thread();
		ts_->start();
		m_list_thread_.push_back(ts_);
	}
}

bool thread_pool::Run(job* job_,void* param_)
{
	bool bflag_ = false;
	work_thread* ts_;
	for(auto wkth_:m_list_thread_)
	{
		if(wkth_->get_status())
		{
			bflag_ = true;
			ts_ = wkth_;
			break;
		}
	}
	if(bflag_ == false)
	{
		if(m_list_thread_.size() < m_thread_max_)
		{
			ts_ = new work_thread();
			ts_->start();
			m_list_thread_.push_back(ts_);
		}
		else
		{
			return false;
		}
	}

	return ts_->set_job(job_,param_);
}

thread_pool_async::thread_pool_async(size_t num_)
{
	m_thread_num_ = num_;
	m_thread_max_ = num_ * 2;
	m_list_thread_.clear();
	init();
	release_msg();
}
thread_pool_async::~thread_pool_async()
{
	stop();
	for(auto wkth_:m_list_thread_)
	{
		if(nullptr == wkth_)
			continue;
		while(!wkth_->get_status())
		{
			usleep(100);
		}
		delete wkth_;
		wkth_ = nullptr;
	}
	m_list_thread_.clear();
}

void thread_pool_async::init()
{
	for(size_t i=0;i< m_thread_num_;++i)
	{
		work_thread* ts_;
		ts_ = new work_thread();
		ts_->start();
		m_list_thread_.push_back(ts_);
	}
}

void thread_pool_async::reduce_thread()
{
	if(m_list_thread_.size() - m_thread_num_ <= 2)
	{
		return;
	}
	size_t thread_num_ = 0;
	auto it = m_list_thread_.begin();
	while(it != m_list_thread_.end())
	{
		if((*it)->get_status())
		{
			++thread_num_;
			if(thread_num_ >= m_thread_num_)
			{
				auto wkth_ = *it;
				m_list_thread_.erase(it++);
				delete wkth_;
				wkth_ = nullptr;
				continue;
			}
		}
		++it;
	}
}
void thread_pool_async::release_msg()
{
	atp_message msg;

	while(!m_queue_msg_.empty())
	{
		msg = m_queue_msg_.front();
		if(msg.param_)
		{
			msg.param_ = nullptr;
			msg.job_ = nullptr;
		}
		m_queue_msg_.pop();
	}
}

bool thread_pool_async::Run(job* job_,void* param_)
{
	atp_message msg;
	msg.job_ = job_;
	msg.param_ = param_;
	return push(msg);
}
bool thread_pool_async::push(atp_message& msg)
{
	std::lock_guard<std::mutex> lock(m_mutex_msg_);
	m_queue_msg_.push(msg);
	m_condition_msg_.notify_one();
	return true;
}

bool thread_pool_async::get(atp_message& msg)
{
	if(m_queue_msg_.size() <= 0)
	{
		return false;
	}
	msg = m_queue_msg_.front();
	m_queue_msg_.pop();
	return true;
}

void thread_pool_async::start()
{
	process_thread::start();
}

void thread_pool_async::stop()
{
	release_msg();
	m_condition_msg_.notify_one();
	process_thread::stop();
}

void thread_pool_async::run()
{
	atp_message msg;
	while(is_running())
	{
		if(!get(msg))
		{
			std::unique_lock<std::mutex> lock(m_mutex_msg_);
			m_condition_msg_.wait_for(lock,std::chrono::milliseconds(10));
			continue;
		}
		work_thread* ts_;
		while(nullptr == (ts_ = get_thread()))
		{
			usleep(10);
		}

		ts_->set_job(msg.job_,msg.param_);
	}
}

work_thread* thread_pool_async::get_thread()
{
	work_thread* ts_ = nullptr;
	for(auto wkth_:m_list_thread_)
	{
		if(wkth_->get_status())
		{
			return wkth_;
		}
	}

	if(m_list_thread_.size() < m_thread_max_)
	{
		ts_ = new work_thread();
		ts_->start();
		m_list_thread_.push_back(ts_);
		return ts_;
	}
	else
	{
		return nullptr;
	}
}

}

