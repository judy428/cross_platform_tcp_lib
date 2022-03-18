/*****************************************************************************
 Thread_Pool Copyright (c) 2015. All Rights Reserved.

 FileName: job.h
 Version: 1.0
 Date: 2017-8-16

 History:
 zhujy     2017-8-16   1.0     Create
 ******************************************************************************/
#ifndef JOB_H_
#define JOB_H_

namespace threadpool
{
class job
{
public:
	job() = default;
	~job() = default;
	virtual void* workfun(void* data_) = 0;
};
}
#endif /* JOB_H_ */
