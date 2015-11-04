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
