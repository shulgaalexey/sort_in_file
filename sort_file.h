/**
 * sort_file.h
 * This is a class for sorting a file of items
 */

#include <string>
#include <vector>
#include "common.h"

#ifndef _SORT_FILE_H_INCLUDED_
#define _SORT_FILE_H_INCLUDED_

class bucket;
class bucket_manager;
class resource_monitor;


class sort_file {
protected:
	/* Input file name */
	std::string _input_file_name;
	std::string _output_file_name;

	/* System resource monitor */
	resource_monitor *_resource_mon;

	/* Bucket manager */
	bucket_manager *_bucket_mgr;

	/* Unique string tag
	 * (it is needed for multi-level "cascade" sorting
	 * of huge files in the environment with small memory
	 */
	std::string _tag;

	/* Foe simplicity, the tag is a number of the instance */
	static unsigned int _id;

public:
	sort_file(const std::string &input_file_name,
		  const std::string &output_file_name);
	virtual ~sort_file();
public:
	void start();
	void pause();
	void cancel();
protected:
	void read_input_file();
	void sort_buckets();
	void concatentate_buckets();
protected:
	resource_monitor *get_resource_monitor() const;
	bucket_manager *get_bucket_manager() const;

	friend class bucket;
};

#endif /* _SORT_FILE_H_INCLUDED_ */
