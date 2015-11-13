/**
 * sort_file_async.h
 * This is a class for sorting a file of items in a multithreading mode
 */

#ifndef _SORT_FILE_ASYNC_H_INCLUDED_
#define _SORT_FILE_ASYNC_H_INCLUDED_

#include "sort_file.h"

class job_manager;

class sort_file_async : public sort_file {
protected:
	/* Thread Pull */
	job_manager *_job_mgr;

public:
	sort_file_async(const std::string &input_file_name,
			const std::string &output_file_name);
	virtual ~sort_file_async();
public:
	/*
	 * We can re-use the same algorithm as we have
	 * for single-tread mode.
	 * On the other hand, in a multithreading mode we can implement
	 * pause and cancel services.
	 */
	virtual void pause();
	virtual void resume();
	virtual void cancel();
protected:
	virtual void sort_buckets();
protected:
	job_manager *get_job_manager() const;
};

#endif /* _SORT_FILE_ASYNC_H_INCLUDED_ */
