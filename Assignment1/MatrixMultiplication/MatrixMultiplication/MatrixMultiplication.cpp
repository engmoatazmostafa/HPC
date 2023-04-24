// MatrixMultiplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <mpi.h>
#define N 3
#define PARALLEL_TASK 27 //N ^ 3

int main(int argc, char* argv[])
{
    int A[N][N] = { 
        { 1, 1, 1 },
        { 2, 2, 2 },
        { 3, 3, 3 } };

    int B[N][N] = { 
        { 1, 1, 1 },
        { 2, 2, 2 },
        { 3, 3, 3 } };

    int C[N][N] = { 
            { 0, 0 , 0},
            { 0, 0 , 0},
            { 0, 0 , 0} };

    MPI_Init(&argc, &argv);
    int rank, numberOfProcesses;
    int inputs[PARALLEL_TASK][4];
    int outputs[PARALLEL_TASK];

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    if (rank == 0)
    {
        int nProcIndex = 1;
        for (int nRow = 0; nRow < N; nRow++)
        {
            for (int nColumn = 0; nColumn < N; nColumn++)
            {
                for (int k = 0; k < N; k++) 
                {
                    if (nProcIndex >= numberOfProcesses) break;
                    int input[2] = {
                    A[nRow][k],
                    B[k][nColumn]
                    };
                    MPI_Request request;
                    int nIndex = nProcIndex - 1;
                    MPI_Send(input, 2, MPI_INT, nProcIndex, 0, MPI_COMM_WORLD);

                    inputs[nIndex][0] = input[0];
                    inputs[nIndex][1] = input[1];
                    inputs[nIndex][2] = nRow;
                    inputs[nIndex][3] = nColumn;
                    printf("master process starts child# %d for %d,%d\n", nProcIndex, nRow, nColumn);
                    nProcIndex++;
                }

            }

        }

        for (nProcIndex = 1; nProcIndex <= numberOfProcesses-1; nProcIndex++)
        {
            int nIndex = nProcIndex - 1;
            int output; //0: value, 1: row, 2: column
            MPI_Recv(& output, 1, MPI_INT, nProcIndex, 1, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            C[inputs[nIndex][2]][inputs[nIndex][3]] += output;
        }

        printf("\noutput\n");
        for (int nRow = 0; nRow < N; nRow++)
        {
            for (int nColumn = 0; nColumn < N; nColumn++)
            {
                printf(" %d ", C[nRow][nColumn]);
            }
            printf("\n");
        }
    }
    else if (rank > 0)
    {
        int input[2];
        MPI_Recv(input, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int Z = input[0] * input[1];
        MPI_Send(&Z, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("child process %d replies value=%d\n", rank, Z);
    }
    MPI_Finalize();
    return 0;

}
