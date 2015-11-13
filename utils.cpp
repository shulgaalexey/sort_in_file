/**
* utils.cpp
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/timeb.h>
#include "common.h"
#include "bucket_manager.h"

static bucket_manager reference(NULL);

void test_generate_input_file(const std::string &file_name, const size_t N)
{

	//cout << "Writing data into test file..." << endl;
	std::ofstream f;
	f.open(file_name.c_str(), std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << file_name << std::endl;
		return;
	}
	for(size_t i = 0; i < N; i ++) {
		//DATA_TYPE val = (DATA_TYPE)rand();
		int tmp = reference.get_val_min() +
			rand() % reference.get_val_max();
		if(tmp < 0)
			tmp *= -1;
		DATA_TYPE val = (DATA_TYPE)tmp;
		//val = 3;
		f.write((char *)(&val), sizeof(val));
	}
	f.close();
}

bool is_file_sorted(const std::string &file_name)
{
	std::ifstream f;
	f.open(file_name.c_str(), std::ios::in | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << file_name << std::endl;
		return false;
	}
	DATA_TYPE val = reference.get_val_min();
	DATA_TYPE prev_val = reference.get_val_min();
	bool ok = true;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		if(val < prev_val) {
			ok = false;
			break;
		}
		prev_val = val;
	}
	f.close();
	return ok;
}

bool test_check_file(const std::string &file_name)
{
	return is_file_sorted(file_name);
}

void trace_file(const std::string &file_name)
{
	std::cout << "File " << file_name << " contents:" << std::endl;
	std::ifstream f;
	f.open(file_name.c_str(), std::ios::in | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << file_name << std::endl;
		return;
	}
	f.seekg (0, f.beg);
	DATA_TYPE val = 0;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		std::cout << val << " " ;
	}
	f.close();
	std::cout << std::endl << std::endl;
}

int __get_milli_count()
{
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int __get_milli_span(int nTimeStart)
{
	int nSpan = __get_milli_count() - nTimeStart;
	if (nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

