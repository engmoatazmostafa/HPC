#pragma once
#include <vector>
using namespace std;
enum UnderlyingSortingAlgorithm { Quick, Merge, Binary };
class ArrayProcessor
{
public:
	ArrayProcessor(int argc, char* argv[]);
	void QuickSortRecursive(int* data, int sizeOfArray);
	void BinarySort(int* data, int sizeOfArray);
	void MergeSort(int* data, int sizeOfArray);
	void QuickSort(int* data, int sizeOfArray);
	void LoadBalancedSort(int* data, int sizeOfArray, UnderlyingSortingAlgorithm algorithm);
	void FindMin(int* data, int sizeOfArray);
	void FindMax(int* data, int sizeOfArray);
	void multipleMerge(int intermediateBuf[], int* sendcounts, int* displacements, int numberOfProcesses);

private:
	int _argc;
	char*** _argv;
	void printArray(int* a);
	void FindMinOrMax(int* a, int sizeOfArray, bool getMin);
	void QuickSortRecursive(int arr[], int arr_start, int arr_end, int my_rank, int p_start, int p_end);

	void merge(int arr[], int arr_size, int arr_2[], int arr_2_size);
	void QuickSortRecursive(int arr[], size_t size, int my_rank, int comm_sz);
};

