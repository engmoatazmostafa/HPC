#pragma once
#include <vector>
using namespace std;
class SequentialProcessor
{
public:
	int FindMin(int* a, int size);
	int FindMax(int* a, int size);
	int* merge(int* A, int asize, int* B, int bsize);
	void MergeSort(int* A, int min, int max);
	void QuickSort(int* arr, size_t size);
	int validate(int* arr, size_t size);

	void inline swap(int* a, int* b);
	void binarySort(int data[], int sizeOfArray);
	int binarySearch(int datat[], int item, int low, int high);
private:
	void QuickSortRecursive(int* arr, size_t start, size_t stop);
	size_t QuickSortPartition(int* arr, size_t start, size_t stop);
};

