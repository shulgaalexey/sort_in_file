/**
 * common.h
 */
#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_

#include <climits>

/* Data type of items, stored in files, which is to sorted */
typedef unsigned int DATA_TYPE;

/* The size of the input file */
const size_t INPUT_SIZE =
	//25;
	//100;
	//200;
	//2000;
	//50 * 1000;
	//500 * 1000;
	5 * 1000 * 1000;
	//50 * 1000 * 1000;
	//500 * 1000 * 1000;

/* The maximum number of items, which may be sotred in the memory */
const size_t MAX_ITEM_COUNT_IN_MEMORY_ALLOWED =
	//10;
	//50;
	//100;
	//1000;
	5000;

/* The number of buckets to use while sorting */
const size_t BUCKET_NUMBER =
	//5;
	//10;
	100;

const DATA_TYPE MIN_VALUE = 0;
const DATA_TYPE MAX_VALUE =
	//30;
	//100;
	//1000;
	UINT_MAX;

const size_t TEST_NUMBER =
	//1;
	//2;
	//5;
	10;
	//20;

const bool __debug_trace =
	//true;
	false;



#endif /* _COMMON_H_INCLUDED_ */
