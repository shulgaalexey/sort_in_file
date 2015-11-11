/**
 * common.h
 */
#ifndef _COMMON_H_INCLUDED_
#define _COMMON_H_INCLUDED_

/* Data type of items, stored in files, which is to sorted */
typedef unsigned int DATA_TYPE;

/* The size of the input file */
const size_t INPUT_SIZE =
	//25;
	//200;
	//2000;
	//50000;
	//500000;
	5000000;

/* The maximum number of items, which may be sotred in the memory */
const size_t MAX_ITEM_COUNT_IN_MEMORY_ALLOWED =
	//10;
	//50;
	//100;
	1000;


const bool __debug_trace =
	//true;
	false;

#endif /* _COMMON_H_INCLUDED_ */

