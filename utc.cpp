/**
 * utc.cpp
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sys/timeb.h>
#include "utc.h"
#include "bucket_manager.h"

static bucket_manager reference(NULL);

/*---------------------------------------------------------------------------*/

input_file_generator::input_file_generator(const std::string &input_file_name,
		const size_t item_number)
	: _input_file_name(input_file_name)
	  , _item_number(item_number)
{
}

input_file_generator::~input_file_generator()
{
}


rand_file_geneator::rand_file_geneator(const std::string &input_file_name,
			const size_t item_number)
	: input_file_generator(input_file_name, item_number)
{

}

rand_file_geneator::~rand_file_geneator()
{
}

void rand_file_geneator::start()
{
	std::ofstream f;
	f.open(_input_file_name.c_str(), std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _input_file_name << std::endl;
		return;
	}
	for(size_t i = 0; i < _item_number; i ++) {
		int tmp = reference.get_val_min() +
			rand() % reference.get_val_max();
		if(tmp < 0)
			tmp *= -1;
		DATA_TYPE val = (DATA_TYPE)tmp;
		f.write((char *)(&val), sizeof(val));
	}
	f.close();
}


const_value_file_geneator::const_value_file_geneator(const std::string &input_file_name,
			const size_t item_number,
			const DATA_TYPE &value)
	: input_file_generator(input_file_name, item_number)
	, _value(value)
{
}

const_value_file_geneator::~const_value_file_geneator()
{
}

void const_value_file_geneator::start()
{
	std::ofstream f;
	f.open(_input_file_name.c_str(), std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _input_file_name << std::endl;
		return;
	}
	for(size_t i = 0; i < _item_number; i ++)
		f.write((char *)(&_value), sizeof(_value));
	f.close();
}


sorted_file_geneator::sorted_file_geneator(const std::string &input_file_name,
			const size_t item_number)
	: input_file_generator(input_file_name, item_number)
{
}

sorted_file_geneator::~sorted_file_geneator()
{
}

void sorted_file_geneator::start()
{
	std::ofstream f;
	f.open(_input_file_name.c_str(), std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _input_file_name << std::endl;
		return;
	}

	DATA_TYPE range = reference.get_val_max() + reference.get_val_min();
	if(range <= 0)
		range = 1;
	size_t cnt = _item_number / range;
	if(cnt < 1)
		cnt = 1;

	DATA_TYPE val = reference.get_val_min();
	size_t count_down = cnt;
	for(size_t i = 0; i < _item_number; i ++) {
		f.write((char *)(&val), sizeof(val));
		count_down --;
		if(count_down == 0) {
			count_down = cnt;
			val ++;
		}

	}
	f.close();
}


reverse_sorted_file_geneator::reverse_sorted_file_geneator(const std::string &input_file_name,
			const size_t item_number)
	: input_file_generator(input_file_name, item_number)
{
}

reverse_sorted_file_geneator::~reverse_sorted_file_geneator()
{
}

void reverse_sorted_file_geneator::start()
{
	std::ofstream f;
	f.open(_input_file_name.c_str(), std::ios::out | std::ios::binary);
	if(!f.is_open()) {
		std::cout << "ERROR Opening File: " << _input_file_name << std::endl;
		return;
	}

	DATA_TYPE range = reference.get_val_max() + reference.get_val_min();
	if(range <= 0)
		range = 1;
	size_t cnt = _item_number / range;
	if(cnt < 1)
		cnt = 1;

	DATA_TYPE val = reference.get_val_max();
	size_t count_down = cnt;
	for(size_t i = 0; i < _item_number; i ++) {
		f.write((char *)(&val), sizeof(val));
		count_down --;
		if(count_down == 0) {
			count_down = cnt;
			val --;
		}

	}
	f.close();
}

/*---------------------------------------------------------------------------*/
