#include "SequentialProcessor.h"
#include <malloc.h>
#include <stdio.h>

int SequentialProcessor::FindMin(int* a, int size)
{
    if (size > 0)
    {
        int min = a[0];
        for (int i = 1; i < size; i++) {
            min = min < a[i] ? min : a[i];
        }
        return min;
    }
    else 
    {
        return 0;
    }
}

int SequentialProcessor::FindMax(int* a, int size)
{
    if (size > 0)
    {
        int max = a[0];
        for (int i = 1; i < size; i++) {
            max = max > a[i] ? max : a[i];
        }
        return max;
    }
    else
    {
        return 0;
    }

}

int* SequentialProcessor::merge(int* A, int asize, int* B, int bsize)
{
	int ai, bi, ci, i;
	int* C;
	int csize = asize + bsize;

	ai = 0;
	bi = 0;
	ci = 0;


	C = (int*)malloc(csize * sizeof(int));
	while ((ai < asize) && (bi < bsize)) {
		if (A[ai] <= B[bi]) {
			C[ci] = A[ai];
			ci++; ai++;
		}
		else {
			C[ci] = B[bi];
			ci++; bi++;
		}
	}

	if (ai >= asize)
		for (i = ci; i < csize; i++, bi++)
			C[i] = B[bi];
	else if (bi >= bsize)
		for (i = ci; i < csize; i++, ai++)
			C[i] = A[ai];

	for (i = 0; i < asize; i++)
		A[i] = C[i];
	for (i = 0; i < bsize; i++)
		B[i] = C[asize + i];

	return C;
}

void SequentialProcessor::merge_sort(int* A, int min, int max)
{
	int* C;		
	int mid = (min + max) / 2;
	int lowerCount = mid - min + 1;
	int upperCount = max - mid;

	/* If the range consists of a single element, it's already sorted */
	if (max == min) {
		return;
	}
	else {
		/* Otherwise, sort the first half */
		merge_sort(A, min, mid);
		/* Now sort the second half */
		merge_sort(A, mid + 1, max);
		/* Now merge the two halves */
		C = merge(A + min, lowerCount, A + mid + 1, upperCount);
	}
}

void SequentialProcessor::serial_qsort(int* arr, size_t size)
{
	serial_qsort_rec(arr, 0, size - 1);
}

int SequentialProcessor::validate(int* arr, size_t size)
{
	if (size < 2) {
		return 1;
	}

	size_t i;
	for (i = 0; i < (size - 1); ++i) {
		if (arr[i] > arr[i + 1]) {
			return 0;
		}
	}

	return 1;
}

inline void SequentialProcessor::swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void SequentialProcessor::binarySort(int data[], int sizeOfArray)
{
	int i, loc, j, k, selected;

	for (i = 1; i < sizeOfArray; ++i)
	{
		j = i - 1;
		selected = data[i];

		// find location where selected should be inserted
		loc = binarySearch(data, selected, 0, j);

		// Move all elements after location to create space
		while (j >= loc)
		{
			data[j + 1] = data[j];
			j--;
		}
		data[j + 1] = selected;
	}
}

int SequentialProcessor::binarySearch(int datat[], int item, int low, int high)
{
	while (low <= high) {
		int mid = low + (high - low) / 2;
		if (item == datat[mid])
			return mid + 1;
		else if (item > datat[mid])
			low = mid + 1;
		else
			high = mid - 1;
	}

	return low;
}

void SequentialProcessor::serial_qsort_rec(int* arr, size_t start, size_t stop)
{
	if (start < stop) {
		size_t p = partition(arr, start, stop);
		if (p > 0) {
			serial_qsort_rec(arr, start, p - 1);
		}
		if (p < stop) {
			serial_qsort_rec(arr, p + 1, stop);
		}
	}
}

size_t SequentialProcessor::partition(int* arr, size_t start, size_t stop)
{
	int pivot = arr[stop];

	size_t i = start - 1, j;
	for (j = start; j < stop; ++j) {
		if (arr[j] <= pivot) {
			++i;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[stop]);

	return i + 1;
}

