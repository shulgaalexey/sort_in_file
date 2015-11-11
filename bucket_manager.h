/**
 * bucket_manager.h
 */

#ifndef _BUCKET_MANAGER_H_INCLUDED_
#define _BUCKET_MANAGER_H_INCLUDED_

#include <vector>
#include "common.h"

class bucket;
class resource_monitor;


class bucket_manager {
protected:
	/* TODO: implement bucket_manager */
	std::vector<bucket *> _buckets;

	/* The number of buckets */
	size_t _bucket_number;	/* 5, 10, 100 */

	/* The minimal value of the data */
	DATA_TYPE _val_min;	 /* 0 */

	/* The maximal value of the data */
	DATA_TYPE _val_max;	/* 30, 5000 */

	/* The range of item values in each bucket */
	size_t _bucket_range_size; /* (val_max - val_min) / bucket_count; */

	/* Resource Monitor */
	resource_monitor *_resource_mon;

public:
	bucket_manager(resource_monitor *mon);
	virtual ~bucket_manager();
public:
	void open_buckets(const std::string &tag);
	void close_buckets();
	void fluch_buckets();

public:
	virtual size_t get_bucket_no(const DATA_TYPE &val);

public:
	size_t get_bucket_number() const;
	bucket *get_bucket(size_t bucket_no) const;
	DATA_TYPE get_val_min() const;
	DATA_TYPE get_val_max() const;
	size_t get_bucket_range_size() const;

	void set_bucket_number(size_t bucket_number);
	void set_val_min(const DATA_TYPE &val_min);
	void set_val_max(DATA_TYPE &val_max);
	void set_bucket_range_size(size_t bucket_range_size);

protected:
	resource_monitor *get_resource_monitor() const;
};

#endif /* _BUCKET_MANAGER_H_INCLUDED_ */
