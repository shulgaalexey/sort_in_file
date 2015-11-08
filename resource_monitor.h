/**
 * resource_monitor.h
 */
#ifndef _RESOURCE_MONITOR_H_INCLUDED_
#define _RESOURCE_MONITOR_H_INCLUDED_
#include <map>

class bucket;

class resource_monitor {
	private:
		map<bucket *, int> _buckets;
		size_t _cur_items_in_memory;

	public:
		resource_monitor();
		virtual ~resource_monitor();
	public:
		static resource_monitor *get_ptr();
	public:
		void register_bucket(bucket *b);
		void unregister_bucket(bucket *b);
		void items_added(size_t cnt);
		void items_released(size_t cnt);
	protected:
		void flush_buckets();
};

#endif /* _RESOURCE_MONITOR_H_INCLUDED_ */
