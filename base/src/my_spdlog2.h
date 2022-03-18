#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <time.h>
#include <chrono>
#include <ctime>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no color needed
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace base{
	
class XLogger2
{
public:
	typedef std::shared_ptr<XLogger2> XLogger2Ptr;
public:
	static XLogger2Ptr getInstance()
	{
		// if(m_instance_logger==nullptr){
        //       m_instance_logger = std::shared_ptr<XLogger2>(new XLogger2());
        // }
		static XLogger2Ptr m_instance_logger = std::shared_ptr<XLogger2>(new XLogger2());
        return m_instance_logger;
	}
	
	
	std::shared_ptr<spdlog::logger> getLogger()
	{
		return m_logger;
	}
	
	void InitLog(std::string file_path  ,std::string file_name  ,std::string lev )
	{
		// hardcode log path
		const std::string log_dir = file_path; // should create the folder if not exist
		const std::string logger_name_prefix = file_name;
 
		// decide print to console or log file
		bool console = true;
 
		// decide the log level
		std::string level = lev;
 
		try
		{
			// logger name with timestamp
			int date = NowDateToInt();
			int time = NowTimeToInt();
			//const std::string logger_name = logger_name_prefix + std::to_string(date) + "_" + std::to_string(time);
			logger_name = logger_name_prefix + std::to_string(date) + "_" + std::to_string(time);
			auto full_name = log_dir + "/" + logger_name;
 
			// printf("logger_name:%s",logger_name.c_str());
			//  printf("full_name:%s",full_name.c_str());
			if (console){
				std::cout<<"hello 0"<<std::endl;
				m_logger = spdlog::stdout_color_st(logger_name); // single thread console output faster
				std::cout<<"hello"<<std::endl;
			}
			else{
				m_logger = spdlog::create_async<spdlog::sinks::daily_file_sink_mt>(logger_name,log_dir + "/" + file_name + ".log", 0,2); // 
				//m_logger = spdlog::create_async<spdlog::sinks::rotating_file_sink_mt>(logger_name, log_dir + "/" + logger_name + ".log", 512, 1000);
			}
			// custom format
			std::cout<<"hello 1"<<std::endl;
			m_logger->set_pattern("%Y-%m-%d %H:%M:%S.%f <thread %t> [%l] [%@] %v"); // with timestamp, thread_id, filename and line number
			std::cout<<"hello 2"<<std::endl;
			if (level == "trace")
				m_logger->set_level(spdlog::level::trace);
			else if (level == "debug")
				m_logger->set_level(spdlog::level::debug);
			else if (level == "info")
				m_logger->set_level(spdlog::level::info);
			else if (level == "warn")
				m_logger->set_level(spdlog::level::warn);
			else if (level == "error")
				m_logger->set_level(spdlog::level::err);

			//当遇到错误级别以上的马上刷新到日志
			m_logger->flush_on(spdlog::level::warn);
			//每两秒刷新一次
			spdlog::flush_every(std::chrono::seconds(2));
 
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Log initialization failed: " << ex.what() << std::endl;
		}
		
	}

	~XLogger2()
	{
		// std::cout<<":hello"<<std::endl;
		//spdlog::shutdown();
		spdlog::drop(logger_name);
		//spdlog::drop_all(); // must do this
	}
private:
	// make constructor private to avoid outside instance
	XLogger2()
	{
		//this->InitLog();
		
	}
 
	// void* operator new(size_t size)
	// {}
 
	XLogger2(const XLogger2&) = delete;
	XLogger2& operator=(const XLogger2&) = delete;
 
private:
	std::string logger_name;
	std::shared_ptr<spdlog::logger> m_logger;
	std::shared_ptr<spdlog::logger> m_logger2;
	//static XLogger2Ptr m_instance_logger;
	
};
// XLogger2::XLogger2Ptr XLogger2::m_instance_logger = nullptr;

// use embedded macro to support file and line number
#define XLOG2_TRACE(...) SPDLOG_LOGGER_CALL(XLogger2::getInstance()->getLogger().get(), spdlog::level::trace, __VA_ARGS__)
#define XLOG2_DEBUG(...) SPDLOG_LOGGER_CALL(XLogger2::getInstance()->getLogger().get(), spdlog::level::debug, __VA_ARGS__)
#define XLOG2_INFO(...) SPDLOG_LOGGER_CALL(XLogger2::getInstance()->getLogger().get(), spdlog::level::info, __VA_ARGS__)
#define XLOG2_WARN(...) SPDLOG_LOGGER_CALL(XLogger2::getInstance()->getLogger().get(), spdlog::level::warn, __VA_ARGS__)
#define XLOG2_ERROR(...) SPDLOG_LOGGER_CALL(XLogger2::getInstance()->getLogger().get(), spdlog::level::err, __VA_ARGS__)


//#define SLOG_ERROR(modle,code,...) SPDLOG_LOGGER_CALL(XLogger2::getInstance()->getLogger().get(), spdlog::level::err,"[" modle ","#code"]:" __VA_ARGS__)

// #define SLOG_TRACE(modle,...) XLOG_TRACE("[" modle "]:" __VA_ARGS__)
// #define SLOG_DEBUG(modle,...) XLOG_DEBUG("[" modle "]:" __VA_ARGS__)
// #define SLOG_INFO(modle,...) XLOG_INFO("[" modle "]:" __VA_ARGS__)
// #define SLOG_WARN(modle,...) XLOG_WARN("[" modle "]:" __VA_ARGS__)
// // #define SLOG_ERROR(modle,code,...) XLOG_ERROR("[" modle "," #code "]:" __VA_ARGS__)

#define SLOG2_TRACE(modle,...) XLOG2_TRACE("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG2_DEBUG(modle,...) XLOG2_DEBUG("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG2_INFO(modle,...) XLOG2_INFO("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG2_WARN(modle,...) XLOG2_WARN("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG2_ERROR(modle,code,...) XLOG2_ERROR("[" + std::string(modle) + "," + std::to_string(code) + "]:" + __VA_ARGS__)
 
//  void  InitLog2(std::string file_path = "./log",std::string file_name = "test_",std::string level = "debug"){
// 	 (XLogger2::getInstance())->InitLog(file_path,file_name,level);
//  }

}

