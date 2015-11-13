/**
 * sort_file_async.cpp
 */

#include <iostream>
#include "sort_file_async.h"
#include "bucket.h"
#include "bucket_manager.h"
#include "job_manager.h"


extern void trace_file(const std::string &file_name);

sort_file_async::sort_file_async(const std::string &input_file_name,
				 const std::string &output_file_name)
	: sort_file(input_file_name, output_file_name)
	  , _job_mgr(NULL)
{
	_job_mgr = new job_manager();
}

sort_file_async::~sort_file_async()
{
	/* Release the Job Manager */
	if(_job_mgr)
		delete _job_mgr;
	_job_mgr = NULL;
}

void sort_file_async::pause()
{
	get_job_manager()->pause();
}

void sort_file_async::resume()
{
	get_job_manager()->resume();
}

void sort_file_async::cancel()
{
	get_job_manager()->stop();
}

void sort_file_async::sort_buckets()
{
	/* Perform bucket sort in multithreaded mode */
	get_job_manager()->start();
	for(size_t i = 0; i < get_bucket_manager()->get_bucket_number(); i ++) {
		bucket *b = get_bucket_manager()->get_bucket(i);
		if(b)
			get_job_manager()->add(new sort_bucket_job(b));
	}

	get_job_manager()->stop();
}

job_manager *sort_file_async::get_job_manager() const
{
	return _job_mgr;
}
