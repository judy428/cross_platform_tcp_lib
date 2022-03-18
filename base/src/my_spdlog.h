#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <time.h>
#include <chrono>
#include <ctime>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h" // or "../stdout_sinks.h" if no color needed
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace base{
	static inline int NowDateToInt()
{
	time_t now;
	time(&now);
	// choose thread save version in each platform
	tm p;
#ifdef _WIN32
	localtime_s(&p, &now);
#else
	localtime_r(&now, &p);
#endif // _WIN32
	int now_date = (1900 + p.tm_year) * 10000 + (p.tm_mon + 1) * 100 + p.tm_mday;
	return now_date;
}
 
static inline int NowTimeToInt()
{
	time_t now;
	time(&now);
	// choose thread save version in each platform
	tm p;
#ifdef _WIN32
	localtime_s(&p, &now);
#else
	localtime_r(&now, &p);
#endif // _WIN32
 
	int now_int = p.tm_hour * 10000 + p.tm_min * 100 + p.tm_sec;
	return now_int;
}

class XLogger
{
public:
	typedef std::shared_ptr<XLogger> XLoggerPtr;
public:
	static XLoggerPtr getInstance()
	{
		// if(m_instance_logger==nullptr){
        //       m_instance_logger = std::shared_ptr<XLogger>(new XLogger());
        // }
		static XLoggerPtr m_instance_logger = std::shared_ptr<XLogger>(new XLogger());
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
		bool console = false;
 
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
			if (console)
				m_logger = spdlog::stdout_color_st(logger_name); // single thread console output faster
			else{
				m_logger = spdlog::create_async<spdlog::sinks::daily_file_sink_mt>(logger_name,log_dir + "/" + file_name + ".log", 0,2); // 
				//m_logger = spdlog::create_async<spdlog::sinks::rotating_file_sink_mt>(logger_name, log_dir + "/" + logger_name + ".log", 512, 1000);
			}
			// custom format
			m_logger->set_pattern("%Y-%m-%d %H:%M:%S.%f <thread %t> [%l] [%@] %v"); // with timestamp, thread_id, filename and line number
 
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

	~XLogger()
	{
		// std::cout<<":hello"<<std::endl;
		//spdlog::shutdown();
		spdlog::drop(logger_name);
		//spdlog::drop_all(); // must do this
	}
private:
	// make constructor private to avoid outside instance
	XLogger()
	{
		//this->InitLog();
		
	}
 
	// void* operator new(size_t size)
	// {}
 
	XLogger(const XLogger&) = delete;
	XLogger& operator=(const XLogger&) = delete;
 
private:
	std::string logger_name;
	std::shared_ptr<spdlog::logger> m_logger;
	//static XLoggerPtr m_instance_logger;
	
};
// XLogger::XLoggerPtr XLogger::m_instance_logger = nullptr;

// use embedded macro to support file and line number
#define XLOG_TRACE(...) SPDLOG_LOGGER_CALL(XLogger::getInstance()->getLogger().get(), spdlog::level::trace, __VA_ARGS__)
#define XLOG_DEBUG(...) SPDLOG_LOGGER_CALL(XLogger::getInstance()->getLogger().get(), spdlog::level::debug, __VA_ARGS__)
#define XLOG_INFO(...) SPDLOG_LOGGER_CALL(XLogger::getInstance()->getLogger().get(), spdlog::level::info, __VA_ARGS__)
#define XLOG_WARN(...) SPDLOG_LOGGER_CALL(XLogger::getInstance()->getLogger().get(), spdlog::level::warn, __VA_ARGS__)
#define XLOG_ERROR(...) SPDLOG_LOGGER_CALL(XLogger::getInstance()->getLogger().get(), spdlog::level::err, __VA_ARGS__)


//#define SLOG_ERROR(modle,code,...) SPDLOG_LOGGER_CALL(XLogger::getInstance()->getLogger().get(), spdlog::level::err,"[" modle ","#code"]:" __VA_ARGS__)

// #define SLOG_TRACE(modle,...) XLOG_TRACE("[" modle "]:" __VA_ARGS__)
// #define SLOG_DEBUG(modle,...) XLOG_DEBUG("[" modle "]:" __VA_ARGS__)
// #define SLOG_INFO(modle,...) XLOG_INFO("[" modle "]:" __VA_ARGS__)
// #define SLOG_WARN(modle,...) XLOG_WARN("[" modle "]:" __VA_ARGS__)
// // #define SLOG_ERROR(modle,code,...) XLOG_ERROR("[" modle "," #code "]:" __VA_ARGS__)

#define SLOG_TRACE(modle,...) XLOG_TRACE("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG_DEBUG(modle,...) XLOG_DEBUG("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG_INFO(modle,...) XLOG_INFO("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG_WARN(modle,...) XLOG_WARN("["+ std::string(modle) + "]:" + __VA_ARGS__)
#define SLOG_ERROR(modle,code,...) XLOG_ERROR("[" + std::string(modle) + "," + std::to_string(code) + "]:" + __VA_ARGS__)
 
//  void  InitLog2(std::string file_path = "./log",std::string file_name = "test_",std::string level = "debug"){
// 	 (XLogger::getInstance())->InitLog(file_path,file_name,level);
//  }

}

