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
// For formating bucket names
#include <sstream>
// For sorting each bucket
#include <algorithm>
#include "common.h"
using namespace std;
vector<fstream *> _buckets(BUCKET_COUNT, NULL);
string getnerate_bucket_name(const size_t bucket_no) {
	stringstream ss;
	ss << "bucket_" << bucket_no << ".dat";
	return ss.str();
}
void open_buckets() {
	for(size_t i = 0; i < _buckets.size(); i ++) {
		fstream *f = new fstream();
		if(!f) {
			cout << "ERROR: not enough memory" << endl;
			return;
		}
		const string bucket_file_name = getnerate_bucket_name(i);
		f->open(bucket_file_name.c_str(),
				ios::out | ios::binary);
		//ios::in | ios::out | ios::binary);
		if(!f->is_open()) {
			cout << "ERROR Opening File: " << bucket_file_name
				<< endl;
			return;
		}
		_buckets[i] = f;
	}
}
void close_buckets() {
	for(size_t i = 0; i < _buckets.size(); i ++) {
		fstream *f = _buckets[i];
		if(!f)
			continue;
		f->close();
		delete f;
		_buckets[i] = NULL;
	}
}
void sort_bucket(size_t bucket_no) {
	//cout << endl << "Sorting Buckets:" << endl;
	fstream f;
	const string bucket_file_name = getnerate_bucket_name(bucket_no);
	f.open(bucket_file_name.c_str(), ios::in | ios::out | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << bucket_file_name
			<< endl;
		return;
	}
	if(f) {
		// get length of file:
		f.seekg (0, f.end);
		const int length = f.tellg() / sizeof(int);
		f.seekg (0, f.beg);
		//cout << "Reading " << length << " ints... " << endl;
		if(length > 0) {
			int *buffer = new int [length];
			// read data as a block:
			//f.read ((char *)buffer, length);
			DATA_TYPE val = 0;
			for(int i = 0; i < length; i ++) {
				f.read((char *)(&val), sizeof(val));
				buffer[i] = val;
			}
			if (f) {
				vector<int> bucket_data(buffer, buffer + length);
				sort(bucket_data.begin(), bucket_data.end());
				f.seekg (0, f.beg);
				//f.write((const char *)bucket_data.data(), length);
				for(size_t i = 0; i<bucket_data.size(); i ++) {
					val = bucket_data[i];
					f.write((char *)(&val), sizeof(val));
				}
			} else {
				cout << "ERROR: only " << f.gcount()
					<< " could be read" << endl;
			}
			delete[] buffer;
		}
		f.close();
	}
}
void sort_buckets() {
	for(size_t i = 0; i < _buckets.size(); i ++)
		sort_bucket(i);
}
int get_bucket_no(int val) {
	return val / BUCKET_RANGE_SIZE;
}
void add_to_bucket(int bucket_no, DATA_TYPE val) {
	/*
	 * This part must be optimized:
	 * each bucket should have a quie to write
	 * and the total size of memory for all bucket ques
	 * should not exceed the available system memory
	 */
	fstream *f = _buckets[bucket_no];
	if(!f)
		return;
	f->write((char *)(&val), sizeof(val));
}
void read_input_file(const string &file_name) {
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
		int bucket_no = get_bucket_no(val);
		add_to_bucket(bucket_no, val);
	}
	f.close();
}
void generate_input_file(const string &file_name, const size_t N) {
	//cout << "Writing data into test file..." << endl;
	ofstream f;
	f.open(file_name.c_str(), ios::out | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return;
	}
	for(size_t i = 0; i < N; i ++) {
		//DATA_TYPE val = (DATA_TYPE)rand();
		int tmp = VAL_MIN + rand() % VAL_MAX;
		if(tmp < 0)
			tmp *= -1;
		DATA_TYPE val = (DATA_TYPE)tmp;
		f.write((char *)(&val), sizeof(val));
	}
	f.close();
}
void concatentate_buckets(const string &result_file_name) {
	ofstream r;
	r.open(result_file_name.c_str(), ios::out | ios::binary);
	if(!r.is_open()) {
		cout << "ERROR Opening File: " << result_file_name << endl;
		return;
	}
	for(size_t i = 0; i < _buckets.size(); i ++) {
		ifstream b;
		const string bucket_file_name = getnerate_bucket_name(i);
		b.open(bucket_file_name.c_str(),
				ios::in | ios::out | ios::binary);
		if(!b.is_open()) {
			cout << "ERROR Opening File: " << bucket_file_name
				<< endl;
			return;
		}
		if(b) {
			// get length of file:
			b.seekg (0, b.end);
			const int length = b.tellg();
			b.seekg (0, b.beg);
			char *buffer = new char [length];
			b.read (buffer, length);
			if (b) {
				r.write(buffer, length);
			} else {
				cout << "ERROR: only " << b.gcount()
					<< " could be read" << endl;
			}
			b.close();
			delete[] buffer;
		}
	}
	r.close();
}
void trace_file(const string &file_name) {
	cout << "File " << file_name << " contents:" << endl;
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
		cout << val << " " ;
	}
	f.close();
	cout << endl << endl;
}
bool check_file(const string &file_name) {
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
		close_buckets();
		if(__debug_trace)
			for(size_t i = 0; i < BUCKET_COUNT; i ++)
				trace_file(getnerate_bucket_name(i));
		sort_buckets();
		if(__debug_trace)
			for(size_t i = 0; i < BUCKET_COUNT; i ++)
				trace_file(getnerate_bucket_name(i));
		concatentate_buckets("result.dat");
		if(__debug_trace)
			trace_file("result.dat");
		if(!check_file("result.dat")) {
			cout << "ERROR OF SORTING!!!" << endl;
			trace_file("test.dat");
		}
	}
	return 0;
}

