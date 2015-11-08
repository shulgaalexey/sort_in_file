/**
 * resource_monitor.cpp
 */
#include "resource_monitor.h"
#include "bucket.h"

resource_monitor::resource_monitor()
{
}

resource_monitor::~resource_monitor()
{
}

resource_monitor *resource_monitor::get_ptr()
{
	static resource_monitor mon;
	return &mon;
}

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
	if(_cur_mem_usage > MAX_ITEM_COUNT_IN_MEMORY_ALLOWED)
		flush_buckets();
}

void resource_monitor::items_released(int cnt)
{
	if(_cur_items_in_memory <= cnt)
		_cur_items_in_memory = 0;
	else
		_cur_items_in_memory -= cnt;

}

void resource_monitor::flush_buckets()
{
	for(map<bucket *, int>::iterator it = _buckets.begint();
			it != _buckets.end(); ++it) {
		bucket *b = *it->first;
		if(b && (it->second == 1))
			b->flush();
	}
}
