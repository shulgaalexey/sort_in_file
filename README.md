Sorting in the File
===================

Overview
--------

This is a demo implementation of a module, sorting a file of something.

### Main requirements
* the file may be small, big or very big
* the file may contain unsigned integers or something else
* the sorting may be performed in a single-treaded or multi-threaded mode


### Discussion of a basic sorting approach
The module implements a conception of Bucket Sort, which is defined as following:
BucketSort(arr[], n)
* Create n empty buckets
* For each arr[i] insert arr[i] into corresponding bucket[B(arr[i])],
  where B is function to detect the index of the bucket
  in which to put the item
* Sort buckets individually
* Concatenate buckets

Note, Counting Sort might be applied as well, but for the realy huge files,
as believed, the Bucket Sort would be more reliable solution.

### Details of the task
Sort a binary file of unsigned 32-bit integers in ascending order in the
assumption that the file size is significantly larger than available memory.

Expected result
C++ source code with a project or a make file of a console application,
that is built with VS 2010/2013 or gcc(g++) 4.6/4.8
The application may assume the execution on a semi-idle multi-core
64bit OS with plenty of HD space.


Design View
-----------

General data flow is as following
<img src="https://github.com/shulgaalexey/sort_in_file/blob/master/doc/data_flow.png" alt="Data Flow" style="width:500px"/>

Data flow in multithreaded environment is following
<img src="https://github.com/shulgaalexey/sort_in_file/blob/master/doc/multithread_data_flow.png" alt="Multithread Data Flow" style="width:500px"/>


In theory it might be developed as a uniform framework for both single thread and multi thread environments
<img src="https://github.com/shulgaalexey/sort_in_file/blob/master/doc/sort_conceptual_workflow.png" alt="Sort Conceptual Workflow" style="width:500px"/>


How to use
----------
The product of the project is a pair of classes, performing sort of binary files.
* sort_file - sorting a file in a singlethread mode

```
#include <string>
#include "sort_file.h"

// Defining input and output file names
const std::string input_file_name = "test.dat";
const std::string output_file_name = "result.dat";

// Sorting a file (single thread)
sort_file s(input_file_name, output_file_name);
s.start();
```


* sort_file_async - sorting a file in a multithread mode

```
#include <string>
#include "sort_file_async.h"

// Defining input and output file names
const std::string input_file_name = "test.dat";
const std::string output_file_name = "result.dat";

// Sorting a file (multithread)
sort_file_async s(input_file_name, output_file_name);
s.start();
```


How to build
-----------
g++ -Wall -o test sort_in_file.cpp sort_file.cpp bucket.cpp resource_monitor.cpp bucket_manager.cpp sort_file_async.cpp job_manager.cpp scope_mutex.cpp utils.cpp utc.cpp -lpthread

g++/gcc 4.8.1

Note: use -g for debug in gdb


How to run
----------
./test


Experiments
-----------

### Comparison of singlethread and multithread performance

Tests demonstrated that multithreaded variant is about 35% faster than a
singlethreaded one.
Test conditions were following:
 * input file item number: 5 000 000
 * item value range: 0 ... UINT_MAX
 * bucket number: 100
 * maximum number of items in memory: 5000
 * number of test runs of each sorting solution 20
 * input files were generated using standard rand() implementation

### Huge file sorting performance test
Sorting a huge file of 500 000 000 items took 5 min on my machine
 * used asynchronous sorting solution
 * used the same item range, bucket number and memory restrictions
 as in previous experiment
 * input file was generated using standard rand() implementation


Reference
---------
Bucket Sort algorithm


TODO
----
* more tests (small imput, huge imput, input consisting a number of single value, etc.)
* code coverage
* code clean-up
* wrap all sort related classes in "sf" workspace

