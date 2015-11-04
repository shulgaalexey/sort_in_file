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

Workflow
--------
<img src="https://github.com/shulgaalexey/sort_in_file/blob/master/doc/sort_workflow.png" alt="Sort Workflow" style="width:500px"/>

How to build
-----------


How to run
----------


Reference
---------


TODO
----
