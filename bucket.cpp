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

using namespace std;

extern void trace_file(const string &file_name);

bucket::bucket(const std::string &file_name)
	: _file_name(file_name)
{
	/* Clear the bucket file if exists */
	remove(_file_name.c_str());

	/* Register the bucket in the resource monitor */
	resource_monitor::get_ptr()->register_bucket(this);
}

bucket::~bucket()
{
	/* Clear the bucket file if exists */
	remove(_file_name.c_str());

	resource_monitor::get_ptr()->unregister_bucket(this);
}

std::string bucket::generate_name(const size_t bucket_no) {
	stringstream ss;
	ss << "bucket_" << bucket_no << ".dat";
	return ss.str();
}

void bucket::add(const DATA_TYPE &item)
{
	_cache.push_back(item);
	/* Notify the global resource monitor, that one more item was added */
	resource_monitor::get_ptr()->items_added(1);
}

void bucket::sort()
{
	fstream f;
	f.open(_file_name.c_str(), ios::in | ios::out | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << _file_name
			<< endl;
		return;
	}
	if(f) {
		/* get length of file */
		f.seekg (0, f.end);
		const int length = f.tellg() / sizeof(int);
		f.seekg (0, f.beg);

		if(length > 0) {
			/* Read the bucket into memory */
			vector<int> bucket_data(length, 0);
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
		}
		f.close();
	}
}

void bucket::read(char **buffer, size_t *length)
{
	ifstream b;
	b.open(_file_name.c_str(), ios::in | ios::binary);
	if(!b.is_open()) {
		cout << "ERROR Opening File: " << _file_name
			<< endl;
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
	trace_file(_file_name);
}

void bucket::flush()
{
	fstream f;
	f.open(_file_name.c_str(), ios::in | ios::binary | ios::app);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << _file_name << endl;
		return;
	}
	if(f) {
		f.seekg (0, f.end);

		for(size_t i = 0; i < _cache.size(); i ++) {
			DATA_TYPE val = _cache[i];
			f.write((char *)(&val), sizeof(val));
		}

		f.close();
	}

	/* Clear the cache */
	_cache.clear();

	/* Notify the global resource monitor, that one more item was added */
	resource_monitor::get_ptr()->items_released(_cache.size());
}

