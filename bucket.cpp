/**
 * bucket.cpp
 */
#include "bucket.h"
#include "resource_monitor.h"

bucket::bucket(const std::string &file_name)
	: _file_name(file_name)
{
	/* TODO: clear the bucket file if exists */

	/* Register the bucket in the resource monitor */
	resource_monitor::get_ptr()->register_bucket(this);
}

bucket::~bucket()
{
	resource_monitor::get_ptr()->unregister_bucket(this);
}

void bucket::add(const DATA_TYPE &item)
{
	_cache.push_back(item);
	/* Notify the global resource monitor, that one more item was added */
	_resource_mon->items_added(1);
}

void bucket::sort()
{
}

void bucket::trace(const std::string &message)
{
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
		// get length of file:
		f.seekg (0, f.end);

		for(size_t i = 0; i < _chache.size(); i ++) {
			DATA_TYPE val = _chache[i];
			f.write((char *)(&val), sizeof(val));
		}

		f.close();
	}

	/* Clear the cache */
	_chache.clear();

	/* Notify the global resource monitor, that one more item was added */
	_resource_mon->items_added(_cache.size());
}

