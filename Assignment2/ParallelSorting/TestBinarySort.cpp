#include "TestBinarySort.h"
#include "ArrayProcessor.h"
// C program for implementation of
// binary insertion sort
#include <iostream>
using namespace std;

//// A binary search based function
//// to find the position
//// where item should be inserted
//// in a[low..high]
//int binarySearch(int datat[], int item, int low, int high)
//{
//    while (low <= high) {
//        int mid = low + (high - low) / 2;
//        if (item == datat[mid])
//            return mid + 1;
//        else if (item > datat[mid])
//            low = mid + 1;
//        else
//            high = mid - 1;
//    }
//
//    return low;
//}
//
//// Function to sort an array a[] of size 'n'
//void binarySort(int data[], int sizeOfArray)
//{
//    int i, loc, j, k, selected;
//
//    for (i = 1; i < sizeOfArray; ++i)
//    {
//        j = i - 1;
//        selected = data[i];
//
//        // find location where selected should be inserted
//        loc = binarySearch(data, selected, 0, j);
//
//        // Move all elements after location to create space
//        while (j >= loc)
//        {
//            data[j + 1] = data[j];
//            j--;
//        }
//        data[j + 1] = selected;
//    }
//}

// Driver Code
int main(int argc, char** argv)
{
    int a[]
        = { 37, 23, 1, 17, 12, 72, 31, 46, 100, 88, 54 };
    int n = sizeof(a) / sizeof(a[0]), i;
    ArrayProcessor* proc = new ArrayProcessor(argc, argv);
    proc->BinarySort(a, n);

    //cout << "Sorted array: \n";
    //for (i = 0; i < n; i++)
    //    cout << " " << a[i];

    return 0;
}

// this code is contribution by shivanisinghss2110