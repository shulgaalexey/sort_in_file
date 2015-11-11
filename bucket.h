/*
 * bucket.h
 */

#ifndef _BUCKET_H_INCLUDED_
#define _BUCKET_H_INCLUDED_

#include <fstream>
#include <vector>
#include "common.h"

class resource_monitor;
class bucket_manager;

class bucket {
private:
	std::string _file_name;
	std::vector<DATA_TYPE> _cache;
	resource_monitor *_resource_mon;
	bucket_manager *_bucket_mgr;
public:
	bucket(const std::string &file_name,
	       resource_monitor *mon,
	       bucket_manager *mgr);
	virtual ~bucket();
private:
	bucket() {}
	bucket(const bucket &src);
public:
	void add(const DATA_TYPE &item);
	void sort();
	void flush();
	void read(char **buffer, size_t *length);

	/* Read the whole contents of the bucket
	* (it is convenient for concatenation)
	 */
	void trace(const std::string &message);
public:
	static std::string generate_name(const size_t bucket_no,
					 const std::string &tag);
protected:
	resource_monitor *get_resource_monitor() const;
protected:
	size_t get_bucket_file_length() const;
	void sort_in_memory();
	void sort_in_file();
protected:
	bucket_manager *get_bucket_manager() const;
};


/* TODO: implement empty_bucket */


#endif /* _BUCKET_H_INCLUDED_ */
