/**
 * resource_monitor.h
 */
#ifndef _RESOURCE_MONITOR_H_INCLUDED_
#define _RESOURCE_MONITOR_H_INCLUDED_

#include <iostream>
#include <map>


class bucket;

class resource_monitor {
	private:
		std::map<bucket *, int> _buckets;
		size_t _cur_items_in_memory;

	public:
		resource_monitor();
		virtual ~resource_monitor();
		/* The singletone is nice, but it conflicts with the conception
		 * of cascading file sort when each bucket file can be sorted
		 * with sort_file algorithm
		 */
	/*public:
		static resource_monitor *get_ptr();*/
	public:
		void register_bucket(bucket *b);
		void unregister_bucket(bucket *b);
		void items_added(size_t cnt);
		void items_released(size_t cnt);
		void use_memory(size_t cnt);
		void release_memory(size_t cnt);
		bool enough_memory_for(size_t cnt);
	protected:
		void flush_buckets();
};

#endif /* _RESOURCE_MONITOR_H_INCLUDED_ */
