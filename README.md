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

BucketSort(arr[], n) \n
1. Create n empty buckets \n
2. For each arr[i]: \n
 2.a insert arr[i] into corresponding bucket[B(arr[i])], where B is
 function to detect the index of the bucket in which to put the item \n
3. Sort buckets individually \n
4. Concatenate buckets \n

Workflow
--------


How to build
-----------


How to run
----------


Reference
---------


TODO
----
