/**
 * common.h
 */
#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_

typedef unsigned int DATA_TYPE;

const size_t INPUT_SIZE = 200;
const int BUCKET_COUNT = 10;
const int VAL_MIN = 0;
const int VAL_MAX = 5000;
const int BUCKET_RANGE_SIZE = (VAL_MAX - VAL_MIN) / BUCKET_COUNT;
const size_t MAX_ITEM_COUNT_IN_MEMORY_ALLOWED = 100;
const bool __debug_trace = false;

#endif /* _COMMON_H_INCLUDED_ */

