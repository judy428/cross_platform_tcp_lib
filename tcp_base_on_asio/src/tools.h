/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: tools.h
Version: 1.0
Date: 2016.1.13

History:
tcpAsio     2016.1.13   1.0     Create
******************************************************************************/

#ifndef __BASE_TTOOLS_H__
#define __BASE_TTOOLS_H__

#ifdef _MSC_VER
#include <Windows.h>
#else
#endif
#include "../../base/src/base.h"
namespace tcpAsio {

/*
return value:
			0:successful;
			~0,falied;
*/
long curr_thread_id();
int bind_cpu(uint64 thread_id,int cpu_id);


}

#endif