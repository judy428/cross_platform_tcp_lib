#include "../src/my_spdlog.h"
#include "../src/my_spdlog2.h"
#include <iostream>
#include <ctime>
#include <chrono>

using namespace base;
int main()
{
	// print log test, you can transfer any param to do format
	int param = 1;
	// auto hi = XLogger::getInstance();
	// hi->InitLog();
	std::cout<<"xl begin:"<<std::endl;
	(XLogger::getInstance())->InitLog("./log","log","debug");
	std::cout<<"xl2 begin:"<<std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	(XLogger2::getInstance())->InitLog("./log2","log","info");
	std::cout<<"xl2 end:"<<std::endl;
	clock_t start = clock(); 
	for(auto i = 0;i<30000000;i++){
		XLOG_DEBUG("this is debug log record, param: {}", 0);
		std::string ss("test");
		int errid = 100;
		SLOG_ERROR(ss.c_str(),errid,"this is error log record, param: {},very much:[{}]",std::to_string(++param)," hello,world");
		SLOG_ERROR(ss,2,"this is error log record, param: {},very much:[{}]",std::to_string(++param)," hello,world");
		//SLOG_ERROR("test",errid,"this is error log record, param: {},very much:[{}]",std::to_string(++param)," hello,world");
		SLOG_INFO(ss,"xxx");
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	 clock_t end  = clock();
    double programTimes = ((double) end -start) / CLOCKS_PER_SEC;
    std::cout<<programTimes<<std::endl;
	return 0;
	while(true){
		XLOG_TRACE("this is trace log record, param: {}", ++param); // int type param is ok
		XLOG_DEBUG("this is debug log record, param: {}", ++param);
		XLOG_INFO("this is info log record, param: {}", ++param);
		XLOG_WARN("this is warn log record, param: {}", double(++param)); // double type param is ok
		XLOG_ERROR("this is error log record, param: {}", std::to_string(++param)); // string type param is ok
		//SLOG_ERROR("test",2,"this is error log record, param: {}%s", std::to_string(++param));
		//sleep(5);
	}
	//time.Sleep(time.Second * 5);	
 
	return 0;
}

