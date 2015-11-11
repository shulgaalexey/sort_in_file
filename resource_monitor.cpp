/**
 * resource_monitor.cpp
 */
#include "resource_monitor.h"
#include "bucket.h"


resource_monitor::resource_monitor()
	: _cur_items_in_memory(0)
{
}

resource_monitor::~resource_monitor()
{
}

/*resource_monitor *resource_monitor::get_ptr()
{
	static resource_monitor mon;
	return &mon;
}*/

void resource_monitor::register_bucket(bucket *b)
{
	_buckets[b] = 1;
}

void resource_monitor::unregister_bucket(bucket *b)
{
	_buckets[b] = 0;
}

void resource_monitor::items_added(size_t cnt)
{
	_cur_items_in_memory += cnt;
	if(_cur_items_in_memory > MAX_ITEM_COUNT_IN_MEMORY_ALLOWED)
		flush_buckets();
}

void resource_monitor::items_released(size_t cnt)
{
	if(_cur_items_in_memory <= cnt)
		_cur_items_in_memory = 0;
	else
		_cur_items_in_memory -= cnt;

}

void resource_monitor::flush_buckets()
{
	for(std::map<bucket *, int>::iterator it = _buckets.begin();
			it != _buckets.end(); ++it) {
		bucket *b = it->first;
		if(b && (it->second == 1))
			b->flush();
	}
}

bool resource_monitor::enough_memory_for(size_t cnt)
{
	return true;
	return ((_cur_items_in_memory + cnt)
		<= MAX_ITEM_COUNT_IN_MEMORY_ALLOWED);
}

void resource_monitor::use_memory(size_t cnt)
{
		_cur_items_in_memory += cnt;
}

void resource_monitor::release_memory(size_t cnt)
{
	if(_cur_items_in_memory <= cnt)
		_cur_items_in_memory = 0;
	else
		_cur_items_in_memory -= cnt;
}
