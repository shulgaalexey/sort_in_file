/**
 * bucket.cpp
 */
#include <iostream>
#include <stdio.h>
#include <algorithm>

// For formating bucket names
#include <sstream>

#include "bucket.h"
#include "resource_monitor.h"
#include "bucket_manager.h"

// For secondary sorting of a bucket file
#include "sort_file.h"

extern void trace_file(const std::string &file_name);
bool is_file_sorted(const std::string &file_name);

bucket::bucket(const std::string &file_name,
	       resource_monitor *mon,
	       bucket_manager *mgr)
	: _file_name(file_name)
	  , _resource_mon(mon)
	  , _bucket_mgr(mgr)
{
	/* Clear the bucket file if exists */
	remove(_file_name.c_str());

	/* Register the bucket in the resource monitor */
	get_resource_monitor()->register_bucket(this);
}

bucket::~bucket()
{
	/* Clear the bucket file if exists */
	remove(_file_name.c_str());

	get_resource_monitor()->unregister_bucket(this);
}

std::string bucket::generate_name(const size_t bucket_no,
				  const std::string &tag)
{
	std::stringstream ss;
	ss << "bucket_" << tag << "_" << bucket_no << ".dat";
	return ss.str();
}

void bucket::add(const DATA_TYPE &item)
{
	_cache.push_back(item);
	/* Notify the global resource monitor, that one more item was added */
	get_resource_monitor()->items_added(1);
}

size_t bucket::get_bucket_file_length() const
{
	std::fstream f;
	f.open(_file_name.c_str(),
	       std::ios::in | std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		/*std::cout << "ERROR Opening File: " << _file_name
			<< std::endl;*/
		return 0;
	}
	if(f) {
		/* get length of file */
		f.seekg (0, f.end);
		const int length = f.tellg() / sizeof(int);
		f.seekg (0, f.beg);
		f.close();
		return length;
	}
	return 0;
}

void bucket::sort()
{
	/* Check the length of bucket file in order to decide if we can sort in
	*  in memory or should run the sort_file algorithm over the bucket file
	*/
	const size_t bucket_file_length = get_bucket_file_length();
	if(bucket_file_length == 0)
		return; /* the bucket is empty, nothing to sort */
	if(get_resource_monitor()->enough_memory_for(bucket_file_length))
		sort_in_memory();
	else
		sort_in_file();
}

void bucket::sort_in_file()
{
	/*
	 * Optimization: check if the bucket file is sorted already.
	 * it is possible, because the value range of items in the bucket is
	 * limited and all items may be the same.
	 */
	if(is_file_sorted(_file_name))
		return;

	sort_file s(_file_name, _file_name);

	const size_t cur_bucket_number =
		get_bucket_manager()->get_bucket_number();
	s.get_bucket_manager()->set_bucket_number(cur_bucket_number * 2);
	s.start();
}

void bucket::sort_in_memory()
{
	std::fstream f;
	f.open(_file_name.c_str(),
	       std::ios::in | std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _file_name
			<< std::endl;
		return;
	}
	if(f) {
		/* get length of file */
		f.seekg (0, f.end);
		const int length = f.tellg() / sizeof(int);
		f.seekg (0, f.beg);

		/* If the length of the file is too long,
		 * then run the sort_file algorithm once again
		 * Othervise, read the file into memory and sort using standerd
		 * tools.
		 */

		if(length > 0) {

			get_resource_monitor()->use_memory(length);

			/* TODO: read whole buffer instead of reading items
			 *  on-by-one
			 */

			/* Read the bucket into memory */
			std::vector<int> bucket_data(length, 0);
			DATA_TYPE val = 0;
			for(int i = 0; i < length; i ++) {
				f.read((char *)(&val), sizeof(val));
				bucket_data[i] = val;
			}

			/* Sort the bucket */
			std::sort(bucket_data.begin(), bucket_data.end());

			/* Write the bucket back into file */
			f.seekg (0, f.beg);
			for(size_t i = 0; i<bucket_data.size(); i ++) {
				val = bucket_data[i];
				f.write((char *)(&val), sizeof(val));
			}


			get_resource_monitor()->release_memory(length);
		}
		f.close();
	}
}

void bucket::read(char **buffer, size_t *length)
{
	const size_t bucket_file_length = get_bucket_file_length();
	if(bucket_file_length == 0)
		return;

	std::ifstream b;
	b.open(_file_name.c_str(), std::ios::in | std::ios::binary);
	if(!b.is_open()) {
		std::cout << "ERROR Opening File: " << _file_name
			<< std::endl;
		return;
	}
	if(b) {
		b.seekg(0, b.end);
		*length = b.tellg();
		b.seekg(0, b.beg);
		*buffer = new char [*length];
		b.read(*buffer, *length);
		b.close();
	}
}

void bucket::trace(const std::string &message)
{
	const size_t bucket_file_length = get_bucket_file_length();
	if(bucket_file_length == 0)
		return;

	trace_file(_file_name);
}

void bucket::flush()
{
	if(_cache.empty())
		return; /* The bucket is not needed to be flushed */

	std::fstream f;
	f.open(_file_name.c_str(),
	       std::ios::out | std::ios::binary | std::ios::app);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _file_name << std::endl;
		return;
	}
	if(f) {
		//f.seekg (0, f.end);

		for(size_t i = 0; i < _cache.size(); i ++) {
			DATA_TYPE val = _cache[i];
			f.write((char *)(&val), sizeof(val));
		}
		f.close();
	}

	/* Notify the global resource monitor, that one more item was added */
	get_resource_monitor()->items_released(_cache.size());

	/* Clear the cache */
	_cache.clear();
}

resource_monitor *bucket::get_resource_monitor() const
{
	return _resource_mon;
}


bucket_manager *bucket::get_bucket_manager() const
{
	return _bucket_mgr;
}
