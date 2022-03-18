/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: main.cpp
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/

#include "threadpool/job.h"
#include "threadpool/threadmanager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

using namespace std;
using namespace threadpool;

class test_job :public job
{
public:
	virtual void* workfun(void* data_)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		cout<<"threadid:"<<std::this_thread::get_id()<<"param:"<< static_cast<char*>(data_)<<endl;
		return 0;
	}
};

int main()
{
	char data_[] = "hello,world";
	thread_manager* tm_ = new thread_manager(2);
	test_job tj_;
	for(int i = 0;i< 10;++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		cout<<i<<"rd,value:"<<tm_->Run(&tj_,data_)<<endl;
	}
	cout<<"0.0"<<endl;
	delete tm_;
	tm_ = NULL;
	cout<<"1.0"<<endl;

	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout<<"finish"<<endl;
	thread_manager_async* tms_ = new thread_manager_async(2);

	for(int i = 0;i< 10;++i)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		cout<<i<<"rd,value:"<<tms_->Run(&tj_,data_)<<endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	delete tms_;
	tms_ = NULL;
	return 0;
}


