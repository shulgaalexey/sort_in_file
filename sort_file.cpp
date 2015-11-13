/**
 * sort_file.cpp
 */

#include "sort_file.h"
#include "bucket.h"
#include "resource_monitor.h"
#include "bucket_manager.h"

// For formating bucket names
#include <sstream>

extern void trace_file(const std::string &file_name);

unsigned int sort_file::_id = 1;

sort_file::sort_file(const std::string &input_file_name,
		     const std::string &output_file_name)
	: _input_file_name(input_file_name)
	  , _output_file_name(output_file_name)
	  , _resource_mon(NULL)
	  , _bucket_mgr(NULL)
{
	/* Generate unique tag */
	std::stringstream ss;
	ss << _id;
	sort_file::_id ++;
	_tag = ss.str();

	/* Create a resource monitor for this instance of sorting algorithm */
	_resource_mon = new resource_monitor();

	/* Create a bucket manager */
	_bucket_mgr = new bucket_manager(_resource_mon);
}

sort_file::~sort_file()
{
	/* Release all buckets */
	if(_bucket_mgr) {
		_bucket_mgr->close_buckets();
		delete _bucket_mgr;
	}
	_bucket_mgr = NULL;


	/* Destroy the resource monitor */
	if(_resource_mon)
		delete _resource_mon;
	_resource_mon = NULL;
}

void sort_file::start()
{
	if(__debug_trace)
		trace_file(_input_file_name);

	/* 1. Prepare buckets for sorting */
	get_bucket_manager()->open_buckets(_tag);

	/* 2. Read input file and distribute values among buckets */
	read_input_file();

	/* 2.1 Fix bucket contents */
	get_bucket_manager()->fluch_buckets();

	if(__debug_trace)
		for(size_t i = 0;
		    i < get_bucket_manager()->get_bucket_number(); i ++)
			trace_file(bucket::generate_name(i, _tag));

	/* 3. Sort buckets individually */
	sort_buckets();

	if(__debug_trace)
		for(size_t i = 0;
		    i < get_bucket_manager()->get_bucket_number(); i ++)
			trace_file(bucket::generate_name(i, _tag));

	/* 4. Concatenate all buckets into an output */
	concatentate_buckets();

	/* 5. Close all buckets */
	get_bucket_manager()->close_buckets();

	if(__debug_trace)
		trace_file(_output_file_name);

}

void sort_file::pause()
{
	/* TODO: */
}

void sort_file::resume()
{
	/* TODO: */
}

void sort_file::cancel()
{
	/* TODO: */
}

void sort_file::read_input_file()
{
	std::ifstream f;
	f.open(_input_file_name.c_str(), std::ios::in | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _input_file_name
			<< std::endl;
		return;
	}
	f.seekg (0, f.beg);
	DATA_TYPE val = 0;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		const size_t bucket_no = get_bucket_manager()->get_bucket_no(val);
		bucket *b = get_bucket_manager()->get_bucket(bucket_no);
		if(b)
			b->add(val);
	}
	f.close();
}

void sort_file::sort_buckets()
{
	for(size_t i = 0; i < get_bucket_manager()->get_bucket_number(); i ++) {
		bucket *b = get_bucket_manager()->get_bucket(i);
		if(b)
			b->sort();
	}
}

void sort_file::concatentate_buckets()
{
	/*std::ofstream r;
	r.open(_output_file_name.c_str(), std::ios::out | std::ios::binary);
	if(!r.is_open()) {
		std::cout << "ERROR Opening File: " << _output_file_name
			<< std::endl;
		return;
	}
	for(size_t i = 0; i < get_bucket_manager()->get_bucket_number(); i ++) {
		bucket *b = get_bucket_manager()->get_bucket(i);
		if(!b)
			continue;

		char *buffer = NULL;
		size_t length = 0;
		b->read(&buffer, &length);
		if(!buffer)
			continue;
		r.write(buffer, length);
		delete [] buffer;
	}
	r.close();*/


	/* Remove the output file if it exists */
	remove(_output_file_name.c_str());

	for(size_t i = 0; i < get_bucket_manager()->get_bucket_number(); i ++) {
		bucket *b = get_bucket_manager()->get_bucket(i);
		if(!b || (b->get_bucket_file_length() == 0))
			continue;

		/* Perform a concatenation of bucket data to an output file */
		file_concatenator fc(_output_file_name, b->get_file_name());
		fc.start();
	}
}

resource_monitor *sort_file::get_resource_monitor() const
{
	return _resource_mon;
}

bucket_manager *sort_file::get_bucket_manager() const
{
	return _bucket_mgr;
}

