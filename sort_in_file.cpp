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

// For generating test input
#include <stdlib.h>
#include <time.h>

// System constants
#include "common.h"

// File sorting algorithm
#include "sort_file.h"
#include "sort_file_async.h"


extern void test_generate_input_file(const std::string &file_name,
				     const size_t N);

extern void trace_file(const std::string &file_name);

extern bool test_check_file(const std::string &file_name);

int __get_milli_count();

int __get_milli_span(int nTimeStart);

int main()
{
	srand(time(NULL));

	const int start_time = __get_milli_count();

	for(size_t test = 0; test < TEST_NUMBER; test ++) {
		std::cout << "Test: " << test << "..." << std::endl;

		const std::string input_file_name = "test.dat";
		const std::string output_file_name = "result.dat";

		/* 1. Prepare test file with items to be sorted */
		test_generate_input_file(input_file_name, INPUT_SIZE);
		if(test_check_file(input_file_name))
			std::cout << "WOOOAA input file is sorted already"
				<< std::endl;

		/* 2. Run sort algorithm
		 * The synchronous alg is about 15% worse than
		 * asynchronous one */
		if(test % 2) {	/* switch both algs */
		//if(false) {		/* run only async */
		//if(true) {		/* run only sync */
			std::cout << "sorting in a single thread......"
				<< std::endl;
			sort_file s(input_file_name, output_file_name);
			s.start();
		} else {
			std::cout << "sorting in multi threads......"
				<< std::endl;
			sort_file_async s(input_file_name, output_file_name);
			s.start();
		}

		/* 3. Check if sort was done correct */
		if(test_check_file(output_file_name))
			std::cout << "OK" << std::endl;
		else {
			std::cout << "ERROR OF SORTING!!!" << std::endl;
			trace_file(input_file_name);
			trace_file(output_file_name);
			break;
		}
	}

	const int milsec = __get_milli_span(start_time);
	std::cout << "Tests finished in " << milsec << " milsec" << std::endl;

	return 0;
}
