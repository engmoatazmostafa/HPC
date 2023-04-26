#pragma once
#include <vector>
using namespace std;

class ArrayProcessor
{
public:
	ArrayProcessor(int argc, char* argv[]);
	void MergeSort(int* data, int sizeOfArray);
	void BinarySort(int* data, int sizeOfArray);
	void FindMin(int* data, int sizeOfArray);
	void FindMax(int* data, int sizeOfArray);
private:
	int _argc;
	char*** _argv;
	void printArray(int* a);
	void FindMinOrMax(int* a, int sizeOfArray, bool getMin);
	void merge_sort_rec(int arr[], int arr_start, int arr_end, int my_rank, int p_start, int p_end);

	void merge(int arr[], int arr_size, int arr_2[], int arr_2_size);
	void merge_sort(int arr[], size_t size, int my_rank, int comm_sz);
};

