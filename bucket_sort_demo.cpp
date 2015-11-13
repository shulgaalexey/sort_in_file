#include <iostream>
#include <queue>
using namespace std;
const int INPUT_SIZE = 20;
const int BUCKET_K = 10;
void print(int *input)
{
	for ( int i = 0; i < INPUT_SIZE; i++ )
		cout << input[i] << " ";
	cout << endl;
}
int hash(int n)
{
	return n/5;
	//return (n % BUCKET_K);
}
void doinsertionsortforbucket(int* input, int len)
{
	while( len-- > 0) {
		if (input[len] > input[len+1]) {
			int tmp = input[len];
			input[len] = input[len+1];
			input[len+1] = tmp;
		} else
			return;
	}
}
void bucketsort(int* input)
{
	queue<int> *buckets[BUCKET_K];
	for ( int i = 0; i < BUCKET_K; i++ )
		buckets[i] = new queue<int>;
	// Hash the input and insert the content into appropriate bucket based on hash.
	for (int i=0;i<INPUT_SIZE;i++)
	{
		int hashValue = hash(input[i]);
		if (hashValue >= BUCKET_K)
			hashValue = BUCKET_K-1;
		buckets[hashValue]->push(input[i]);
	}
	// extract the content from each of the buckets in order.
	// using insertion sort
	int outputidx = 0;
	for ( int i = 0; i < BUCKET_K; i++ )
	{
		if (buckets[i]->size() == 1) {
			input[outputidx++] = buckets[i]->front();
			cout << buckets[i]->front() << " | ";
			buckets[i]->pop();
		}
		if (buckets[i]->size() > 1)
		{
			while (!buckets[i]->empty())
			{
				input[outputidx] = buckets[i]->front();
				doinsertionsortforbucket(input, outputidx);
				cout << buckets[i]->front() << " ";
				buckets[i]->pop();
				outputidx++;
			}
			cout << "| ";
		}
	}
	// clear buckets.
	for ( int i = 0; i < BUCKET_K; i++ )
		delete buckets[i];
}
int main()
{
	int input[INPUT_SIZE] = { 25, 44, 13, 34, 27, 11, 4, 9, 45, 33, 27, 28, 42,
		6, 49, 31, 37, 23, 14, 41 };
	cout << "Input: ";
	print(input);
	cout << "Bucketed list: " ;
	bucketsort(input);
	cout << "\nOutput: ";
	print(input);
	return 0;
}
