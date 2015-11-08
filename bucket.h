/*
 * bucket.h
 */

#ifndef _BUCKET_H_INCLUDED_
#define _BUCKET_H_INCLUDED_
#include <fstream>
#include <vector>
#include "common.h"

class bucket {
	private:
		std::string _file_name;
		vector<DATA_TYPE> _cache;
	public:
		bucket(const std::string &file_name);
		virtual ~bucket();
	private:
		bucket() {}
		bucket(const bucket &src);
	public:
		void add(const DATA_TYPE &item);
		void sort();
		void trace(const std::string &message);
	protected:
		void flush();
};

#endif /* _BUCKET_H_INCLUDED_ */
