/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: tools.cpp
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2016.1.13   1.0     Create
******************************************************************************/


#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#include <sched.h>
#endif
#include "../../base/src/thread.h"
#include "../../base/src/trace.h"

namespace tcpAsio {

long curr_thread_id()
{
#ifdef _MSC_VER
	return GetCurrentThreadId();
#else
	return (long)pthread_self();
#endif
}


int bind_cpu(uint64_t thread_id,int cpu_id)
{
	// TRACE_INFO("base","thread_id:%ld,cpu_id:%d",thread_id,cpu_id);
#ifdef _MSC_VER
	return SetThreadAffinityMask(&thread_id,1 << cpu_id) != 0 ? 0:-1;
#else
	cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(cpu_id,&cpu_set);
	return pthread_setaffinity_np(thread_id,sizeof(cpu_set),&cpu_set);
#endif
}

}