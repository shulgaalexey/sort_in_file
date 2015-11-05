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
#include <vector>

// For generating test input
#include <fstream>
#include <stdlib.h>
#include <time.h>

// For formating bucket names
#include <sstream>

// For sorting each bucket
#include <algorithm>

using namespace std;

const int INPUT_SIZE = 200;
const int BUCKET_COUNT = 10;
const int VAL_MIN = 0;
const int VAL_MAX = 5000;
const int BUCKET_RANGE_SIZE = (VAL_MAX - VAL_MIN) / BUCKET_COUNT;
const bool __debug_trace = false;

vector<fstream *> _buckets(BUCKET_COUNT, NULL);

string getnerate_bucket_name(const size_t bucket_no) {
	stringstream ss;
	ss << "bucket_" << bucket_no << ".dat";
	return ss.str();
}

void open_buckets() {
	for(size_t i = 0; i < _buckets.size(); i ++) {
		fstream *f = new fstream();
		if(!f) {
			cout << "ERROR: not enough memory" << endl;
			return;
		}
		const string bucket_file_name = getnerate_bucket_name(i);
		f->open(bucket_file_name.c_str(),
				ios::out | ios::binary);
		//ios::in | ios::out | ios::binary);
		if(!f->is_open()) {
			cout << "ERROR Opening File: " << bucket_file_name
				<< endl;
			return;
		}
		_buckets[i] = f;
	}
}

void close_buckets() {
	for(size_t i = 0; i < _buckets.size(); i ++) {
		fstream *f = _buckets[i];
		if(!f)
			continue;
		f->close();
		delete f;
		_buckets[i] = NULL;
	}
}

void sort_bucket(size_t bucket_no) {
	//cout << endl << "Sorting Buckets:" << endl;
	fstream f;
	const string bucket_file_name = getnerate_bucket_name(bucket_no);
	f.open(bucket_file_name.c_str(), ios::in | ios::out | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << bucket_file_name
			<< endl;
		return;
	}
	if(f) {
		// get length of file:
		f.seekg (0, f.end);
		const int length = f.tellg() / sizeof(int);
		f.seekg (0, f.beg);
		//cout << "Reading " << length << " ints... " << endl;
		if(length > 0) {
			int *buffer = new int [length];
			// read data as a block:
			//f.read ((char *)buffer, length);
			unsigned int val = 0;
			for(int i = 0; i < length; i ++) {
				f.read((char *)(&val), sizeof(val));
				buffer[i] = val;
			}
			if (f) {
				vector<int> bucket_data(buffer, buffer + length);
				sort(bucket_data.begin(), bucket_data.end());
				f.seekg (0, f.beg);
				//f.write((const char *)bucket_data.data(), length);
				for(size_t i = 0; i<bucket_data.size(); i ++) {
					val = bucket_data[i];
					f.write((char *)(&val), sizeof(val));
				}
			} else {
				cout << "ERROR: only " << f.gcount()
					<< " could be read" << endl;
			}
			delete[] buffer;
		}
		f.close();
	}
}

void sort_buckets() {
	for(size_t i = 0; i < _buckets.size(); i ++)
		sort_bucket(i);
}

int get_bucket_no(int val) {
	return val / BUCKET_RANGE_SIZE;
}

void add_to_bucket(int bucket_no, unsigned int val) {
	/*
	 * This part must be optimized:
	 * each bucket should have a quie to write
	 * and the total size of memory for all bucket ques
	 * should not exceed the available system memory
	 */
	fstream *f = _buckets[bucket_no];
	if(!f)
		return;
	f->write((char *)(&val), sizeof(val));
}

void read_input_file(const string &file_name) {
	ifstream f;
	f.open(file_name.c_str(), ios::in | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return;
	}
	f.seekg (0, f.beg);
	unsigned int val = 0;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		int bucket_no = get_bucket_no(val);
		add_to_bucket(bucket_no, val);
	}
	f.close();
}

void generate_input_file(const string &file_name, const size_t N) {
	//cout << "Writing data into test file..." << endl;
	ofstream f;
	f.open(file_name.c_str(), ios::out | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return;
	}
	for(size_t i = 0; i < N; i ++) {
		//unsigned int val = (unsigned int)rand();
		int tmp = VAL_MIN + rand() % VAL_MAX;
		if(tmp < 0)
			tmp *= -1;
		unsigned int val = (unsigned int)tmp;
		f.write((char *)(&val), sizeof(val));
	}
	f.close();
}

void concatentate_buckets(const string &result_file_name) {
	ofstream r;
	r.open(result_file_name.c_str(), ios::out | ios::binary);
	if(!r.is_open()) {
		cout << "ERROR Opening File: " << result_file_name << endl;
		return;
	}
	for(size_t i = 0; i < _buckets.size(); i ++) {
		ifstream b;
		const string bucket_file_name = getnerate_bucket_name(i);
		b.open(bucket_file_name.c_str(),
				ios::in | ios::out | ios::binary);
		if(!b.is_open()) {
			cout << "ERROR Opening File: " << bucket_file_name
				<< endl;
			return;
		}
		if(b) {
			// get length of file:
			b.seekg (0, b.end);
			const int length = b.tellg();
			b.seekg (0, b.beg);
			char *buffer = new char [length];
			b.read (buffer, length);
			if (b) {
				r.write(buffer, length);
			} else {
				cout << "ERROR: only " << b.gcount()
					<< " could be read" << endl;
			}
			b.close();
			delete[] buffer;
		}
	}
	r.close();
}

void trace_file(const string &file_name) {
	cout << "File " << file_name << " contents:" << endl;
	ifstream f;
	f.open(file_name.c_str(), ios::in | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return;
	}
	f.seekg (0, f.beg);
	unsigned int val = 0;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		cout << val << " " ;
	}
	f.close();
	cout << endl << endl;
}

bool check_file(const string &file_name) {
	ifstream f;
	f.open(file_name.c_str(), ios::in | ios::binary);
	if(!f.is_open()) {
		cout << "ERROR Opening File: " << file_name << endl;
		return false;
	}
	f.seekg (0, f.beg);
	unsigned int val = 0;
	unsigned int prev_val = VAL_MIN;
	bool ok = true;
	while(true) {
		f.read((char *)(&val), sizeof(val));
		if(f.eof())
			break;
		if(val < prev_val) {
			ok = false;
			break;
		}
	}
	f.close();
	return ok;
}

int main()
{
	srand(time(NULL));
	for(int test = 0; test < 50; test ++) {
		cout << "Test: " << test << "..." << endl;
		generate_input_file("test.dat", INPUT_SIZE);
		if(__debug_trace)
			trace_file("test.dat");
		open_buckets();
		read_input_file("test.dat");
		close_buckets();
		if(__debug_trace)
			for(size_t i = 0; i < BUCKET_COUNT; i ++)
				trace_file(getnerate_bucket_name(i));
		sort_buckets();
		if(__debug_trace)
			for(size_t i = 0; i < BUCKET_COUNT; i ++)
				trace_file(getnerate_bucket_name(i));
		concatentate_buckets("result.dat");
		if(__debug_trace)
			trace_file("result.dat");
		if(!check_file("result.dat")) {
			cout << "ERROR OF SORTING!!!" << endl;
			trace_file("test.dat");
		}
	}
	return 0;
}

#if 0
/**
 * The class representing the chunck of the input file.
 * The purpose of this file is to hold the part of the input file.
 */
class input_file_chunc {
	private:
	public:
		intput_file_chinc();
		virtual ~input_file_chunck();
};
/**
 * The class representing Load Balancer, directing scatter jobs.
 * The purpose of this class is to instantiate the optimal amount of scatter
 * jobs and assign input file chuncks to them.
 */
class scatter_lb {
};
/**
 * The class representing Scatter job.
 * The purpose of this class is to scatter input data to buckets.
 */
class scatter_job {
};
/** The class representing the Bucket Adapter.
 * Thepurpose of this class is to provide the access to buckets.
 */
class bucket_adapter {
};
/** The class representing buckets.
 * The purpose of this class is to hold bucket data.
 */
class bucket {
	private:
		int min_item_value; /* Minimal value of items in the bucket */
		int max_item_value; /* Maximal value of items in the bucket */
		//int item_count; /* Number of items, added to the bucket */
		//vector<int> items;/* Items, added to the bucket */
		// allow two options:
		// a. bucket in the memory
		// b. bucket on the disk
		void add_item(int item);
		int get_item(int idx);
};
/** The class representing Bucket Sorting load balancer.
 * The purpose of this class is to instantiate the optimal amount of bucket
 * sorters and assign buckets for them.
 */
class sort_lb {
};
/* The class representing Bucket Sort job.
 * Thepurpose of this class is to sort the items in the assigned buckets.
 */
class sort_job {
	private:
		vector<bucket *> buckets;
};
/** The class representing Load Balancer, directing bucket concatenation jobs.
 * The purpose of this class is to instantiate the optimal amount of concatenate
 * jobs and assigne buckets to them.
 */
class concatenate_lb {
};
/** The class representing sorted Bucket Concatenation job.
 * The purpose of this class is to concatenate sorted buckets into output.
 */
class concatenate_job {
};
/**
 * The main class of sort method.
 * The purpose of this class is to provide interface and implementation of
 * the procedure, sorting the file of something.
 */
class sort {
	private:
		std::string input_file;
		std::string output_file;
	public:
		sort();
		sort(const std::string &input_file,
				const std::string &output_file);
		virtual ~sort();
	public:
		void set_data(const std::string &in_file,
				const std::string &out_file);
		void start();
		void stop();
};
void sort::start() {
	/* 1. Create a number of workers */
	for(int i = 0; i < get_worker_number(); i ++)
		workers.push_back(create_worker());
	/* 2. Run Scatter LB */
	scatter = create_scatter_lb();
	scatter->run();
	/* 3. Run Sort LB */
	sorter = create_sorter_lb();
	sorter->run();
	/* 4. Run Concatenate LB */
	concatenator = create_concatenate_lb();
	concatenator->run();
	/* Release workers */
	for(size_t i = 0; i < workers.size(); i ++)
		workers[i]->release();
}
int main(void) {
	sort s("/tmp/input.dat", "/tmp/sorted.dat");
	s.start();
	return 0;
}
#endif
