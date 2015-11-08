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
		std::vector<DATA_TYPE> _cache;
	public:
		bucket(const std::string &file_name);
		virtual ~bucket();
	private:
		bucket() {}
		bucket(const bucket &src);
	public:
		void add(const DATA_TYPE &item);
		void sort();
		void flush();
		void read(char **buffer, size_t *length);
		void trace(const std::string &message);
	public:
		static std::string generate_name(const size_t bucket_no);
};

#endif /* _BUCKET_H_INCLUDED_ */
