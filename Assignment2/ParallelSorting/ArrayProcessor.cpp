#include "ArrayProcessor.h"
#include "SequentialProcessor.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <malloc.h>
#include <cstdlib>
using namespace std;

ArrayProcessor::ArrayProcessor(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;
}

void ArrayProcessor::MergeSort(int* data, int sizeOfArray)
{
    int my_rank, comm_sz;

    MPI_Init(&_argc, _argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



    merge_sort(data, sizeOfArray, my_rank, comm_sz);

    if (my_rank == 0) {
        SequentialProcessor seqProcessor;

        if (seqProcessor.validate(data, sizeOfArray)) {
            printf("[Info] Validation successful!\n");
        }
        else {
            printf("[Error] Validation not successful :(\n");

        }
        //print_array(data, sizeOfArray);
        printf("\t");
        int i;
        for (i = 0; i < sizeOfArray; ++i) {
            printf("%d ", data[i]);
        }
        printf("\n");
        //free(arr);
    }

    MPI_Finalize();
}

void ArrayProcessor::BinarySort(int* data, int sizeOfArray)
{
    int rank, numberOfProcesses; // for storing this process' rank, and the number of processes

    MPI_Init(&_argc, _argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    int* sendcounts;            // array describing how many elements to send to each process
    int* displacements;                // array describing the displacements where each segment begins
    int rem = (sizeOfArray) % numberOfProcesses; // elements remaining after division among processes
    int sum = 0;                // Sum of counts. Used to calculate displacements
    //int rec_buf[10000];          // buffer where the received data should be stored
    int *rec_buf;          // buffer where the received data should be stored
    int minSplitSize = sizeOfArray / numberOfProcesses;
    int maxSplitSize = minSplitSize;
    int* intermediateBuf;



    intermediateBuf = (int*)malloc(sizeof(int) * sizeOfArray);
    sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses);
    displacements = (int*)malloc(sizeof(int) * numberOfProcesses);
    // calculate send counts and displacements
    for (int i = 0; i < numberOfProcesses; i++) {
        sendcounts[i] = minSplitSize;
        if (rem > 0) {
            maxSplitSize = minSplitSize + 1;
            sendcounts[i]++;
            rem--;
        }

        displacements[i] = sum;
        sum += sendcounts[i];
    }
    rec_buf = (int*)malloc(sizeof(int) * maxSplitSize);
    int partition_size = 0;

    
    MPI_Bcast(&maxSplitSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // divide the data among processes as described by sendcounts and displs
    MPI_Scatterv(data, sendcounts, displacements, MPI_INT, rec_buf, maxSplitSize, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\n");

    //each process do sequential sort on its own partition
    SequentialProcessor seqProc;
    seqProc.binarySort(rec_buf, sendcounts[rank]);


    MPI_Gatherv(rec_buf, sendcounts[rank], MPI_INT, intermediateBuf, sendcounts, displacements, MPI_INT, 0, MPI_COMM_WORLD);

    //int baseSize = sendcounts[0];
    //for (int i = 0; i < numberOfProcesses - 1; i++) {
    //    merge(intermediateBuf, baseSize, intermediateBuf + baseSize, sendcounts[i + 1]);
    //    baseSize += sendcounts[i + 1];
    //}

    if (rank == 0)
    {
        int baseSize = sendcounts[0];
        for (int i = 0; i < numberOfProcesses - 1; i++) {
            merge(intermediateBuf, baseSize, intermediateBuf + baseSize, sendcounts[i + 1]);
            baseSize += sendcounts[i + 1];
        }


        //printf("\n");
        printf("sorted buf:");

        for (int i = 0; i < sizeOfArray; i++)
        {
            printf(" %d ", intermediateBuf[i]);
        }

        printf("\n");
    }


    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    free(sendcounts);
    free(displacements);

}

void ArrayProcessor::FindMin(int* data, int sizeOfArray)
{
    FindMinOrMax(data, sizeOfArray, true);

}

void ArrayProcessor::FindMax(int* data, int sizeOfArray)
{
    FindMinOrMax(data, sizeOfArray, false);
}

void ArrayProcessor::printArray(int* a)
{
	int sizeOfInput = sizeof(a);
	for (int i = 0; i < sizeOfInput; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
}

void ArrayProcessor::FindMinOrMax(int* data, int sizeOfArray, bool getMin)
{
    int rank, numberOfProcesses; // for storing this process' rank, and the number of processes

    MPI_Init(&_argc, _argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    int* sendcounts;            // array describing how many elements to send to each process
    int* displacements;                // array describing the displacements where each segment begins
    int rem = (sizeOfArray) % numberOfProcesses; // elements remaining after division among processes
    int sum = 0;                // Sum of counts. Used to calculate displacements
    int rec_buf[100];          // buffer where the received data should be stored
    int minSplitSize = sizeOfArray / numberOfProcesses;
    int maxSplitSize = minSplitSize;
    int rootValue = 0;


    sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses);
    displacements = (int*)malloc(sizeof(int) * numberOfProcesses);

    // calculate send counts and displacements
    for (int i = 0; i < numberOfProcesses; i++) {
        sendcounts[i] = minSplitSize;
        if (rem > 0) {
            maxSplitSize = minSplitSize + 1;
            sendcounts[i]++;
            rem--;
        }

        displacements[i] = sum;
        sum += sendcounts[i];
    }
    if (0 == rank) {

        for (int i = 0; i < numberOfProcesses; i++) {
            printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displacements[i]);
        }
    }

    // divide the data among processes as described by sendcounts and displs
    MPI_Scatterv(data, sendcounts, displacements, MPI_INT, &rec_buf, maxSplitSize, MPI_INT, 0, MPI_COMM_WORLD);

    int tmpValue = rec_buf[0];
    for (int i = 1; i < sendcounts[rank]; i++) {
        if (getMin)
        {
            tmpValue = tmpValue < rec_buf[i] ? tmpValue : rec_buf[i];
        }
        else
        {
            tmpValue = tmpValue > rec_buf[i] ? tmpValue : rec_buf[i];

        }
    }
    if (getMin)
    {
        MPI_Reduce(&tmpValue, &rootValue, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    }
    else 
    {
        MPI_Reduce(&tmpValue, &rootValue, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    }


    if (rank == 0)
    {
        printf("\nRoot value:%d.\n", rootValue);
    }
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    free(sendcounts);
    free(displacements);
}

void ArrayProcessor::merge_sort_rec(int arr[], int arr_start, int arr_end, int my_rank, int p_start, int p_end)
{
    MPI_Status status;
    if ((p_end - p_start) <= 1) {
        //Base case
        //Perform quicksort on local list
        SequentialProcessor seqProcessor;
        seqProcessor.serial_qsort(arr + arr_start, arr_end - arr_start);
    }
    else {
        int split = p_start + (p_end - p_start) / 2 + ((p_end - p_start) % 2);
        int lower_p_start = p_start, lower_p_end = split,
            upper_p_start = split, upper_p_end = p_end;
        int arr_split = arr_start + (arr_start + arr_end) / 2;
        int* scratch = NULL;
        int split_size = arr_end - arr_split;

        //Split array in half and send to other process
        if (my_rank == p_start) {
            //Send upper half of array
            MPI_Send(arr + arr_start + arr_split, arr_end - arr_split, MPI_INT, split,
                0, MPI_COMM_WORLD);
        }
        else if (my_rank == split) {
            scratch = (int*)malloc(split_size * sizeof(int));

            //Receive upper half of array
            MPI_Recv(scratch, split_size, MPI_INT, p_start, 0, MPI_COMM_WORLD, &status);
        }

        //Recurse
        if (my_rank < split) {
            //Lower-half processes
            merge_sort_rec(arr, arr_start, arr_split, my_rank, p_start, split);
        }
        else {
            //Upper-half processes
            merge_sort_rec(scratch, 0, split_size, my_rank, split, p_end);
        }

        //Merge both sorted halves
        if (my_rank == p_start) {
            scratch = (int*)malloc(split_size * sizeof(int));

            //Receive sorted half
            MPI_Recv(scratch, split_size, MPI_INT, split, 0, MPI_COMM_WORLD, &status);

            //Merge both sorted arrays
            merge(arr + arr_start, arr_split - arr_start, scratch, split_size);

            free(scratch);
        }
        else if (my_rank == split) {
            //Send sorted half
            MPI_Send(scratch, split_size, MPI_INT, p_start, 0, MPI_COMM_WORLD);

            free(scratch);
        }
    }
}

void ArrayProcessor::merge(int arr[], int arr_size, int arr_2[], int arr_2_size)
{
    int* scratch = (int*)malloc((arr_size + arr_2_size) * sizeof(int));
    int i_scratch = 0, i_arr = 0, i_arr_2 = 0;

    for (; (i_arr < arr_size) && (i_arr_2 < arr_2_size); ++i_scratch) {
        if (arr[i_arr] < arr_2[i_arr_2]) {
            scratch[i_scratch] = arr[i_arr++];
        }
        else {
            scratch[i_scratch] = arr_2[i_arr_2++];
        }
    }
    if (i_arr < arr_size) {
        memcpy(scratch + i_scratch, arr + i_arr, (arr_size - i_arr) * sizeof(int));
    }
    else if (i_arr_2 < arr_2_size) {
        memcpy(scratch + i_scratch, arr_2 + i_arr_2, (arr_2_size - i_arr_2) * sizeof(int));
    }

    memcpy(arr, scratch, (arr_size + arr_2_size) * sizeof(int));

    free(scratch);
}

void ArrayProcessor::merge_sort(int arr[], size_t size, int my_rank, int comm_sz)
{
    merge_sort_rec(arr, 0, size, my_rank, 0, comm_sz);
}

