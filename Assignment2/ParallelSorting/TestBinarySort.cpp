#include "TestBinarySort.h"
#include "ArrayProcessor.h"
// C program for implementation of
// binary insertion sort
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
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



void testMultipleMerge(ArrayProcessor* proc) {
    int sizeOfArray = 29;
    int numberOfProcesses = 6;
    int* sendcounts;            // array describing how many elements to send to each process
    int* displacements;                // array describing the displacements where each segment begins

    int rem = (sizeOfArray) % numberOfProcesses; // elements remaining after division among processes
    int minSplitSize = sizeOfArray / numberOfProcesses;
    int sum = 0;

    int* intermediateBuf;
    intermediateBuf = (int*)malloc(sizeof(int) * sizeOfArray);
    sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses);
    displacements = (int*)malloc(sizeof(int) * numberOfProcesses);
    for (int i = 0; i < numberOfProcesses; i++) {
        sendcounts[i] = minSplitSize;
        if (rem > 0) {
            sendcounts[i]++;
            rem--;
        }

        displacements[i] = sum;
        sum += sendcounts[i];
    }
    int tmp = 0;
    for (int i = 0; i < numberOfProcesses; i++) {
        for (int j = 0; j < sendcounts[i]; j++ , tmp++)
        {
            intermediateBuf[tmp] = j + 1;
        }
    }
    printf("\nunmerged [");
    for (int i = 0; i < sizeOfArray; i++)
    {
        printf(" %d ", intermediateBuf[i]);

    }
    printf("]\n");

    proc->multipleMerge(intermediateBuf, sendcounts, displacements, numberOfProcesses);
    printf("\nmerged [");
    for (int i = 0; i < sizeOfArray; i++)
    {
        printf(" %d " , intermediateBuf[i]);

    }
    printf("]\n");

}

// Driver Code
int main(int argc, char** argv)
{
    int sizeOfArray = 20;
    int algorithm = 0; //0: quick sort , 1: merge sort, 2: binary sort
    //std::vector<std::string> arguments(argv, argv + argc);
    //for (std::string& s : arguments) {
    //    printf(" %s ", s);
    //}
    int approach = 0; //0: load balanced , 1: recursive
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-size") == 0 && i < argc - 1)
        {
            string str(argv[i + 1]);
            std::stringstream sizeStr(str);
            int tmp;
            sizeStr >> tmp;
            if (sizeStr)
            {
                sizeOfArray = tmp;
            }
        }
        else if (strcmp(argv[i], "-algorithm") == 0 && i < argc - 1)
        {
            if (strcmp(argv[i + 1], "quick") == 0)
            {
                algorithm = 0;
            }
            else if (strcmp(argv[i + 1], "merge") == 0)
            {
                algorithm = 1;
            }
            else if (strcmp(argv[i + 1], "binary") == 0)
            {
                algorithm = 2;
            }
        }
        else if (strcmp(argv[i], "-approach") == 0 && i < argc - 1)
        {
            if (strcmp(argv[i + 1], "loadbalanced") == 0)
            {
                approach = 0;
            }
            else if (strcmp(argv[i + 1], "recursive") == 0)
            {
                approach = 1;
            }
        }

        printf(" %s " , argv[i]);
    }

    int* arr = (int*)malloc(sizeof(int) * sizeOfArray);
    for (int i = 0; i < sizeOfArray; i++)
    {

        arr[i] = rand();
    }
    //int a[]
    //    = { 37, 23, 1, 17, 12, 72, 31, 46, 100, 88, 54 };
    //int n = sizeof(a) / sizeof(a[0]), i;
    ArrayProcessor* proc = new ArrayProcessor(argc, argv);
    switch (algorithm)
    {
    case 0:
        if (approach == 0) {
            proc->QuickSort(arr, sizeOfArray);
        }
        else {
            proc->QuickSortRecursive(arr, sizeOfArray);
        }        break;
    case 1:
        proc->MergeSort(arr, sizeOfArray);
        break;
    case 2:
        proc->BinarySort(arr, sizeOfArray);
        break;
    default:
        proc->QuickSortRecursive(arr, sizeOfArray);
        break;
    }


    return 0;
}

// this code is contribution by shivanisinghss2110