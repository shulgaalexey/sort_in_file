/**
 * utc.h
 */

#ifndef _UTC_H_INCLUDED_
#define _UTC_H_INCLUDED_

#include <string>
#include "commmon.h"


/*---------------------------------------------------------------------------*/

/**
 * This is a basic class for a family of file generators
 * Files will be sorted with implemented sorters
 */
class input_file_generator {
protected:
	const std::string _input_file_name;
	const size_t _item_number;
public:
	input_file_generator(const std::string &input_file_name,
			const size_t item_number);
	virtual ~input_file_generator();
public:
	virtual void start() = 0;
};

/**
 * This class generates a file with a number of random items
 */
class rand_file_geneator : public input_file_generator {
public:
	rand_file_geneator(const std::string &input_file_name,
			const size_t item_number);
	virtual ~rand_file_geneator();
public:
	virtual void start();
};

/**
 * This class generates a file with a number of items  of a single value
 */
class const_value_file_geneator : public input_file_generator {
private:
	DATA_TYPE _value;
public:
	const_value_file_geneator(const std::string &input_file_name,
			const size_t item_number,
			const DATA_TYPE &value);
	virtual ~const_value_file_geneator();
public:
	virtual void start();
};

/**
 * This class generates an already sorted file with a number of items
 */
class sorted_file_geneator : public input_file_generator {
private:
	DATA_TYPE _value;
public:
	sorted_file_geneator(const std::string &input_file_name,
			const size_t item_number);
	virtual ~sorted_file_geneator();
public:
	virtual void start();
};

/**
 * This class generates a sorted file with a number of items, but
 * items are sorted in reverse order
 */
class reverse_sorted_file_geneator : public input_file_generator {
private:
	DATA_TYPE _value;
public:
	reverse_sorted_file_geneator(const std::string &input_file_name,
			const size_t item_number);
	virtual ~reverse_sorted_file_geneator();
public:
	virtual void start();
};

/*---------------------------------------------------------------------------*/


#if 0
/**
 * This is a base class for each test case
 */
class test_case {
protedted:
	std::string _input_file_name;
	std::string _output_file_name;
public:
	test_case(const std::string &input_file_name,
			const std::string &output_file_name,
			const size_t item_number);
	virtual ~test_case();
public:
	virtual bool run();
};


/**
 * This is a base class for a test case where the input file
 * is generated with a number of rand()
 */
class rand_test : public test_case {
private:
	const size_t _item_number;
public:
	rand_test(const std::string &input_file_name,
			const std::string &output_file_name);
	virtual ~rand_test():
public:
	virtual bool run();
};
#endif

#endif /* _UTC_H_INCLUDED_ */
