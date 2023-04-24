#include "IntermediateDataDecomposition.h"
#include <iostream>
#include <mpi.h>
//#define SEND_MULTIPLICATION_REQUEST_TAG 0
//#define RECIEVE_MULTIPLICATION_REQUEST_TAG 1
IntermediateDataDecomposition::IntermediateDataDecomposition(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;
}

void IntermediateDataDecomposition::Multiply(int A[][N], int B[][N], int C[][N])
{
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    if (rank == 0)
    {
        printf("MATRIX MULTIPLICATION USING INTERMEDIATE DATA DECOMPOSITION\n");

        int I[N][N][N];

        //master process
        int nProcIndex = 1;
        for (int k = 0; k < N; k++)
        {
            for (int nRow = 0; nRow < N; nRow++)
            {
                for (int nColumn = 0; nColumn < N; nColumn++)
                {
                    if (nProcIndex >= numberOfProcesses) break;
                    int input[2] = {
                    A[nRow][k],
                    B[k][nColumn]
                    };
                    int result;
                    MPI_Sendrecv(input, 2, MPI_INT, nProcIndex, 0, &result, 1, MPI_INT, nProcIndex, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                    I[k][nRow][nColumn] = result;
                    printf("master process starts child# %d for %d,%d\n", nProcIndex, nRow, nColumn);
                    nProcIndex++;
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);


        printf("\nsummation\n");
        for (int k = 0; k < N; k++)
        {
            for (int nRow = 0; nRow < N; nRow++)
            {
                for (int nColumn = 0; nColumn < N; nColumn++)
                {
                    C[nRow][nColumn] += I[k][nRow][nColumn];
                }
            }
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
        //child process
        int input[2];
        MPI_Recv(input, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int Z = input[0] * input[1];
        MPI_Send(&Z, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("child process %d replies value=%d\n", rank, Z);
        MPI_Barrier(MPI_COMM_WORLD);

    }
    MPI_Finalize();
}
