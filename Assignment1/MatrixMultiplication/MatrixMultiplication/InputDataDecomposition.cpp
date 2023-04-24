#include "InputDataDecomposition.h"
#include <iostream>
#include <mpi.h>

InputDataDecomposition::InputDataDecomposition(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;
}

void InputDataDecomposition::Multiply(int A[][N], int B[][N], int C[][N])  
{
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    if (rank == 0)
    {
        printf("MATRIX MULTIPLICATION USING INPUT DATA DECOMPOSITION\n");

        //master process
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
                    int result;
                    MPI_Sendrecv(input, 2, MPI_INT, nProcIndex, 0, &result, 1, MPI_INT, nProcIndex, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                    C[nRow][nColumn] += result;
                    printf("master process starts child# %d for %d,%d\n", nProcIndex, nRow, nColumn);
                    nProcIndex++;
                }

            }

        }
        MPI_Barrier(MPI_COMM_WORLD);

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

    }
    MPI_Finalize();
}
