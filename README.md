# sort_in_file

Sorting in the File
===================

Overview
--------

This is a demo implementation of a module, sorting a file of something.

Main requirements are following:
* the file may be small, big or very big
* the file may contain integers or something else
* the sorting may be performed in a single-treaded or multi-threaded mode

The module implements a conception of Bucket Sort, which defined as following:

BucketSort(arr[], n)
* Create n empty buckets
* For each arr[i] insert arr[i] into corresponding bucket[B(arr[i])],
  where B is function to detect the index of the bucket
  in which to put the item
* Sort buckets individually
* Concatenate buckets



Detailed task:

Sort a binary file of unsigned 32-bit integers in ascending order in the
assumption that the file size is significantly larger than available memory.

Expected result
C++ source code with a project or a make file of a console application,
that is built with VS 2010/2013 or gcc(g++) 4.6/4.8
The application may assume the execution on a semi-idle multi-core
64bit OS with plenty of HD space.


Workflow
--------
<img src="https://github.com/shulgaalexey/sort_in_file/blob/master/doc/sort_workflow.png" alt="Sort Workflow" style="width:500px"/>

How to build
-----------
g++ -Wall -g -o test sort_in_file.cpp bucket.cpp resource_monitor.cpp utils.cpp

How to run
----------


Reference
---------


TODO
----
