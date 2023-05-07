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


void ArrayProcessor::QuickSort(int* data, int sizeOfArray)
{
    int my_rank, comm_sz;

    MPI_Init(&_argc, _argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);



    if (my_rank ==0 )
    {
        printf("\narray before sorting\t");
        for (int i = 0; i < sizeOfArray; ++i) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    QuickSort(data, sizeOfArray, my_rank, comm_sz);

    if (my_rank == 0) {
        //SequentialProcessor seqProcessor;

        //if (seqProcessor.validate(data, sizeOfArray)) {
        //    printf("\n[Info] Validation successful!\n");
        //}
        //else {
        //    printf("\n[Error] Validation not successful :(\n");

        //}
        //print_array(data, sizeOfArray);
        printf("\narray output from merge sort\t");
        for (int i = 0; i < sizeOfArray; ++i) {
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

void ArrayProcessor::MergeSort(int* data, int sizeOfArray)
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
    int* rec_buf;          // buffer where the received data should be stored
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


    printf("\nProcess: %d sorts array with size: %d , elements:[" , rank , sendcounts[rank]);
    for (int i = 0; i < sendcounts[rank] - 1; i++) {
        printf(" %d ", rec_buf[i]);
    }
    printf("]\n");
    //each process do sequential sort on its own partition
    SequentialProcessor seqProc;
    seqProc.MergeSort(rec_buf, 0, sendcounts[rank] - 1);


    MPI_Gatherv(rec_buf, sendcounts[rank], MPI_INT, intermediateBuf, sendcounts, displacements, MPI_INT, 0, MPI_COMM_WORLD);

    //int baseSize = sendcounts[0];
    //for (int i = 0; i < numberOfProcesses - 1; i++) {
    //    merge(intermediateBuf, baseSize, intermediateBuf + baseSize, sendcounts[i + 1]);
    //    baseSize += sendcounts[i + 1];
    //}

    if (rank == 0)
    {
        //int baseSize = sendcounts[0];
        //for (int i = 0; i < numberOfProcesses - 1; i++) {
        //    merge(intermediateBuf, baseSize, intermediateBuf + baseSize, sendcounts[i + 1]);
        //    baseSize += sendcounts[i + 1];
        //}
        multipleMerge(intermediateBuf, sendcounts, displacements, numberOfProcesses);



        printf("\n");
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

void ArrayProcessor::QuickSortRecursive(int arr[], int arr_start, int arr_end, int my_rank, int p_start, int p_end)
{
    MPI_Status status;
    if ((p_end - p_start) <= 1) {
        //printf("\n base case sequential sort:process_id:%d,  arr_start:%d , size:%d " , my_rank, arr_start, arr_end - arr_start);
        printf("\n sequential array sort on process: %d , size: %d [", my_rank , arr_end - arr_start);
        for (int i = 0; i < arr_end - arr_start; i++)
        {
            printf(" %d ", arr[i + arr_start]);
        }
        printf("]\n");
        //Base case
        //Perform quicksort on local list
        SequentialProcessor seqProcessor;
        seqProcessor.QuickSort(arr + arr_start, arr_end - arr_start);
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
            printf("\n recursive sort on another process:caller process_id:%d, called process id: %d,  arr_start:%d , size:%d", my_rank, split, arr_start + arr_split, arr_end - arr_split);

            //Send upper half of array
            MPI_Send(arr + arr_start + arr_split, arr_end - arr_split, MPI_INT, split,
                0, MPI_COMM_WORLD);
        }
        else if (my_rank == split) {
            //printf("\n child process recieves request to sort :caller process_id:%d, called process id: %d,  size:%d", p_start, my_rank, split_size);
            scratch = (int*)malloc(split_size * sizeof(int));

            //Receive upper half of array
            MPI_Recv(scratch, split_size, MPI_INT, p_start, 0, MPI_COMM_WORLD, &status);
            //printf("\n parallel array sort on process: %d [", my_rank);
            //for (int i = 0; i < split_size; i++)
            //{
            //    printf(" %d ", scratch[i]);
            //}
            //printf("]\n");

        }

        //Recurse
        if (my_rank < split) {
            //Lower-half processes
            QuickSortRecursive(arr, arr_start, arr_split, my_rank, p_start, split);
        }
        else {
            //Upper-half processes
            QuickSortRecursive(scratch, 0, split_size, my_rank, split, p_end);
        }

        //Merge both sorted halves
        if (my_rank == p_start) {
            scratch = (int*)malloc(split_size * sizeof(int));

            //Receive sorted half
            //printf("\n parent process recieves from child :caller process_id:%d, called process id: %d,  size:%d", my_rank, split, split_size);
            MPI_Recv(scratch, split_size, MPI_INT, split, 0, MPI_COMM_WORLD, &status);

            printf("\nmerge output: parent process %d , child process: %d, my array size:%d, split_size: %d", my_rank , split , arr_split - arr_start , split_size);
            printf("\nfirst array:[");
            for (int i = 0; i < arr_split - arr_start; i++)
            {
                printf(" %d ", arr[i + arr_start]);
            }
            printf("]\n");
            printf("\nsecond array:[");
            for (int i = 0; i < split_size; i++)
            {
                printf(" %d ", scratch[i + arr_start]);
            }

            printf("]\n");
            //Merge both sorted arrays
            merge(arr + arr_start, arr_split - arr_start, scratch, split_size);

            free(scratch);
        }
        else if (my_rank == split) {
            //printf("\n child process finishes sorting and send to caller :caller process_id:%d, called process id: %d,  size:%d", p_start, my_rank, split_size);

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

void ArrayProcessor::multipleMerge(int intermediateBuf[], int* sendcounts, int* displacements, int numberOfProcesses)
{
    int step = 1;
    int index1 = 0;
    int size1 = 0;
    int index2 = index1 + step;
    int size2 = 0;
    int tmp = 0;
    int index3 = 0;
    int displacement1 = 0;
    int displacement2 = 0;
    while (step < numberOfProcesses)
    {

        index1 = 0;

        while (index1 < numberOfProcesses)
        {
            index2 = index1 + step;
            index3 = index2 + step;
            size1 = 0;
            tmp = index1;
            while (tmp < index2 && tmp < numberOfProcesses)
            {
                size1 += sendcounts[tmp];
                tmp++;
            }
            size2 = 0;
            tmp = index2;
            while (tmp < index3 && tmp < numberOfProcesses)
            {
                size2 += sendcounts[tmp];
                tmp++;
            }
            if (size1 > 0 && size2 > 0)
            {
                merge(intermediateBuf + displacements[index1], size1, intermediateBuf + displacements[index2], size2);
            }
            else
            {
                break;
            }
            index1 = index3;
        }

        tmp = step;
        step = step * 2;
        if (step > numberOfProcesses)
        {
            index1 = 0;
            index2 = tmp;
            size1 = 0;
            displacement1 = displacements[index1];
            tmp = index1;
            while (tmp < index2 && tmp < numberOfProcesses)
            {
                size1 += sendcounts[tmp];
                tmp++;
            }
            tmp = index2;
            while (tmp < numberOfProcesses)
            {
                size2 = sendcounts[tmp];
                displacement2 = displacements[tmp];
                if (size1 > 0 && size2 > 0)
                {
                    merge(intermediateBuf + displacement1, size1, intermediateBuf + displacement2, size2);
                    size1 += size2;
                }
                tmp++;
            }

        }
    }

}

void ArrayProcessor::QuickSort(int arr[], size_t size, int my_rank, int comm_sz)
{
    QuickSortRecursive(arr, 0, size, my_rank, 0, comm_sz);
}

