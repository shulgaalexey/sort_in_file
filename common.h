/**
 * common.h
 */
#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_

/* Data type of items, stored in files, which is to sorted */
typedef unsigned int DATA_TYPE;

/* The size of the input file */
const size_t INPUT_SIZE =
	25;
	//200;

/* The number of buckets */
const int BUCKET_COUNT =
	5;
	//10;

/* The minimal value of the data */
const int VAL_MIN = 0;

/* The maximal value of the data */
const int VAL_MAX =
	30;
	//5000;

/* The range of item values in each bucket */
const int BUCKET_RANGE_SIZE = (VAL_MAX - VAL_MIN) / BUCKET_COUNT;

/* The maximum number of items, which may be sotred in the memory */
const size_t MAX_ITEM_COUNT_IN_MEMORY_ALLOWED = 50;


const bool __debug_trace = true;

#endif /* _COMMON_H_INCLUDED_ */

