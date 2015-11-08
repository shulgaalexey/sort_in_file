/**
 * utils.cpp
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "common.h"

using namespace std;

void generate_input_file(const string &file_name, const size_t N)
{
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

void trace_file(const string &file_name)
{
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


