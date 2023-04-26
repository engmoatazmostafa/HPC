//#include <mpi.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <fstream>
//#include <vector>
//using namespace std;
//
//#define SIZE 100
//
//int main(int argc, char* argv[])
//{
//    int rank, size; // for storing this process' rank, and the number of processes
//
//    MPI_Init(&argc, &argv);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    int* sendcounts;            // array describing how many elements to send to each process
//    int* displs;                // array describing the displacements where each segment begins
//    int rem = (SIZE) % size; // elements remaining after division among processes
//    int sum = 0;                // Sum of counts. Used to calculate displacements
//    int rec_buf[100];          // buffer where the received data should be stored
//    int partial_min[3] = {0,0,0};
//    int rootMin = 0;
//
//    //// the data to be distributed
//    //char data[SIZE][SIZE] = {
//    //    {'a', 'b', 'c', 'd'},
//    //    {'e', 'f', 'g', 'h'},
//    //    {'i', 'j', 'k', 'l'},
//    //    {'m', 'n', 'o', 'p'}
//    //};
//    int* data;
//
//    sendcounts = (int*)malloc(sizeof(int) * size);
//    displs = (int*)malloc(sizeof(int) * size);
//    data = (int*)malloc(sizeof(int) * SIZE);
//    //partial_min = (int*)malloc(sizeof(int) * size);
//
//    // calculate send counts and displacements
//    for (int i = 0; i < size; i++) {
//        sendcounts[i] = (SIZE) / size;
//        if (rem > 0) {
//            sendcounts[i]++;
//            rem--;
//        }
//
//        displs[i] = sum;
//        sum += sendcounts[i];
//    }
//    for (int i = 0; i < SIZE; i++)
//    {
//        data[i] = 14 + i;
//    }
//    // print calculated send counts and displacements for each process
//    if (0 == rank) {
//
//        for (int i = 0; i < size; i++) {
//            printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
//        }
//    }
//
//    // divide the data among processes as described by sendcounts and displs
//    MPI_Scatterv(data, sendcounts, displs, MPI_INT, &rec_buf, (((SIZE) / size) + 1), MPI_INT, 0, MPI_COMM_WORLD);
//
//    int tmpMin = rec_buf[0];
//    for (int i = 1; i < sendcounts[rank]; i++) {
//        tmpMin = tmpMin > rec_buf[i] ? tmpMin : rec_buf[i];
//    }
//
//    MPI_Reduce(&tmpMin, &rootMin, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
//
//    // print what each process received
//    printf("%d: ", rank);
//    for (int i = 0; i < sendcounts[rank]; i++) {
//        printf("%d\t", rec_buf[i]);
//    }
//    if (rank == 0)
//    {
//
//        printf("\nmin value:%d.\n", rootMin);
//		ofstream logFile;
//		logFile.open("result.txt");
//		if (logFile)
//		{
//            logFile << rootMin;
//
//			logFile.close();
//		}
//        //printf("\nVector of partial min values.\n");
//        //for (int i = 0; i < 3; i++)
//        //{
//        //    printf("%d ", partial_min[i]);
//        //}
//        //printf("\n");
//    }
//    printf("\n");
//    MPI_Barrier(MPI_COMM_WORLD);
//
//    MPI_Finalize();
//
//    free(sendcounts);
//    free(displs);
//
//    return 0;
//}