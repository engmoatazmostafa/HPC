#pragma once
#include <vector>
using namespace std;
class SequentialProcessor
{
public:
	int FindMin(int* a, int size);
	int FindMax(int* a, int size);
	int* merge(int* A, int asize, int* B, int bsize);
	void merge_sort(int* A, int min, int max);
	void serial_qsort(int* arr, size_t size);
	int validate(int* arr, size_t size);

	void inline swap(int* a, int* b);
	void binarySort(int data[], int sizeOfArray);
	int binarySearch(int datat[], int item, int low, int high);
private:
	void serial_qsort_rec(int* arr, size_t start, size_t stop);
	size_t partition(int* arr, size_t start, size_t stop);
};

