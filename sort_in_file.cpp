/*
 * This is a demo implementation of a module, sorting a file of something.
 *
 * Main requirements are following:
 * * the file may be small, big or very big
 * * the file may contain integers or something else
 * * the sorting may be performed in a single-treaded or multi-threaded mode
 * .
 *
 * The module implements a conception of Bucket Sort, which defined as
 * following:
 *
 * BucketSort(arr[], n)
 * 1. Create n empty buckets
 * 2. For each arr[i]:
 * 2.a insert arr[i] into corresponding bucket[B(arr[i])], where B is a
 * function to detect the index of the bucket in which to put the item
 * 3. Sort buckets individually
 * 4. Concatenate buckets
 *
 */
/*
 * This is a demo implementation of a module, sorting a file of something.
 *
 * Main requirements are following:
 * * the file may be small, big or very big
 * * the file may contain integers or something else
 * * the sorting may be performed in a single-treaded or multi-threaded mode
 * .
 *
 * The module implements a conception of Bucket Sort, which defined as
 * following:
 *
 * BucketSort(arr[], n)
 * 1. Create n empty buckets
 * 2. For each arr[i]:
 * 2.a insert arr[i] into corresponding bucket[B(arr[i])], where B is a
 * function to detect the index of the bucket in which to put the item
 * 3. Sort buckets individually
 * 4. Concatenate buckets
 *
 */

#include <iostream>
#include <vector>

// For generating test input
#include <fstream>
#include <stdlib.h>
#include <time.h>


// For sorting each bucket
#include "common.h"

#include "bucket.h"

using namespace std;

extern void trace_file(const string &file_name);
extern void generate_input_file(const string &file_name, const size_t N);

/* TODO: implement bucket_manager */
vector<bucket *> _buckets(BUCKET_COUNT, NULL);

void open_buckets()
{
	for(size_t i = 0; i < _buckets.size(); i ++)
		_buckets[i] = new bucket(bucket::generate_name(i));
}

void close_buckets()
{
	for(size_t i = 0; i < _buckets.size(); i ++) {
		bucket *b = _buckets[i];
		if(b)
			delete b;
		_buckets[i] = NULL;
	}
}

void fluch_buckets()
{
	for(size_t i = 0; i < _buckets.size(); i ++)
		if(_buckets[i])
			_buckets[i]->flush();
}

/* TODO: implement empty_bucket */

void sort_buckets()
{
	for(size_t i = 0; i < _buckets.size(); i ++)
		if(_buckets[i])
			_buckets[i]->sort();
}

size_t get_bucket_no(const DATA_TYPE &val)
{
	return size_t(val / BUCKET_RANGE_SIZE);
}

void read_input_file(const string &file_name)
{
	ifstream f;
	f.open(file_name.c_str(), ios::in | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return;
	}
	f.seekg (0, f.beg);
	DATA_TYPE val = 0;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		size_t bucket_no = get_bucket_no(val);
		if(_buckets[bucket_no])
			_buckets[bucket_no]->add(val);
	}
	f.close();
}

void concatentate_buckets(const string &result_file_name)
{
	ofstream r;
	r.open(result_file_name.c_str(), ios::out | ios::binary);
	if(!r.is_open()) {
		cout << "ERROR Opening File: " << result_file_name << endl;
		return;
	}
	for(size_t i = 0; i < _buckets.size(); i ++) {
		if(!_buckets[i])
			continue;

		char *buffer = NULL;
		size_t length = 0;
		_buckets[i]->read(&buffer, &length);
		r.write(buffer, length);
		delete [] buffer;
	}
	r.close();
}

bool check_file(const string &file_name)
{
	ifstream f;
	f.open(file_name.c_str(), ios::in | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return false;
	}
	f.seekg (0, f.beg);
	DATA_TYPE val = 0;
	DATA_TYPE prev_val = VAL_MIN;
	bool ok = true;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		if(val < prev_val) {
			ok = false;
			break;
		}
	}
	f.close();
	return ok;
}

int main()
{
	srand(time(NULL));

	for(int test = 0; test < 50; test ++) {
		cout << "Test: " << test << "..." << endl;

		generate_input_file("test.dat", INPUT_SIZE);

		if(__debug_trace)
			trace_file("test.dat");

		open_buckets();
		read_input_file("test.dat");
		fluch_buckets();

		if(__debug_trace)
			for(size_t i = 0; i < BUCKET_COUNT; i ++)
				trace_file(bucket::generate_name(i));

		sort_buckets();

		if(__debug_trace)
			for(size_t i = 0; i < BUCKET_COUNT; i ++)
				trace_file(bucket::generate_name(i));

		concatentate_buckets("result.dat");
		close_buckets();

		if(__debug_trace)
			trace_file("result.dat");

		if(!check_file("result.dat")) {
			cout << "ERROR OF SORTING!!!" << endl;
			trace_file("test.dat");
		}
	}
	return 0;
}

