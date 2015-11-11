/**
 * bucket_manager.h
 */

#include <iostream>
#include <climits>
#include "bucket_manager.h"
#include "bucket.h"


bucket_manager::bucket_manager(resource_monitor *mon)
	: _bucket_number(100)	/* 5, 10, 100 */
	  , _val_min(0)
	  , _val_max(UINT_MAX)	/* 30, 100, 1000, UINT_MAX */
	  , _bucket_range_size((_val_max - _val_min) / _bucket_number)
	  , _resource_mon(mon)
{
}

bucket_manager::~bucket_manager()
{
}

void bucket_manager::open_buckets(const std::string &tag)
{
	if(!_buckets.empty())
		close_buckets();

	_buckets.resize(get_bucket_number(), NULL);
	for(size_t i = 0; i < _buckets.size(); i ++)
		_buckets[i] = new bucket(bucket::generate_name(i, tag),
					 get_resource_monitor(),
					 this);
}

void bucket_manager::close_buckets()
{
	for(size_t i = 0; i < _buckets.size(); i ++) {
		bucket *b = _buckets[i];
		if(b)
			delete b;
		_buckets[i] = NULL;
	}
}

void bucket_manager::fluch_buckets()
{
	for(size_t i = 0; i < _buckets.size(); i ++)
		if(_buckets[i])
			_buckets[i]->flush();
}

size_t bucket_manager::get_bucket_no(const DATA_TYPE &val)
{
	return size_t(val / get_bucket_range_size());
}

bucket *bucket_manager::get_bucket(size_t bucket_no) const
{
	if(bucket_no >= _buckets.size()) {
		std::cout << "ERROR: bucket_no is out of range" << std::endl;
		return NULL;
	}
	return _buckets[bucket_no];
}

size_t bucket_manager::get_bucket_number() const
{
	return _bucket_number;
}

DATA_TYPE bucket_manager::get_val_min() const
{
	return _val_min;
}

DATA_TYPE bucket_manager::get_val_max() const
{
	return _val_max;
}

size_t bucket_manager::get_bucket_range_size() const
{
	return _bucket_range_size;
}

void bucket_manager::set_bucket_number(size_t bucket_number)
{
	_bucket_number = bucket_number;
	_bucket_range_size = (_val_max - _val_min) / _bucket_number;
	if(_bucket_range_size < 1)
		_bucket_range_size = 1;
}

void bucket_manager::set_val_min(const DATA_TYPE &val_min)
{
	_val_min = val_min;
}

void bucket_manager::set_val_max(DATA_TYPE &val_max)
{
	_val_max = val_max;
}

void bucket_manager::set_bucket_range_size(size_t bucket_range_size)
{
	_bucket_range_size = bucket_range_size;
}

resource_monitor *bucket_manager::get_resource_monitor() const
{
	return _resource_mon;
}
