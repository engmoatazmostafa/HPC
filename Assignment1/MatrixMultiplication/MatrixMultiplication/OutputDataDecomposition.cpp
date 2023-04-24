#include "OutputDataDecomposition.h"
#include <iostream>
#include <mpi.h>

OutputDataDecomposition::OutputDataDecomposition(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;

}

void OutputDataDecomposition::Multiply(int A[][N], int B[][N], int C[][N])
{
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    if (rank == 0)
    {
        printf("MATRIX MULTIPLICATION USING OUTPUT DATA DECOMPOSITION\n");

        //master process
        int nProcIndex = 1;
        for (int nRow = 0; nRow < N; nRow++)
        {
            for (int nColumn = 0; nColumn < N; nColumn++)
            {
                //one child process per output
                if (nProcIndex >= numberOfProcesses) break;
                int input[N][2];
                for (int k = 0; k < N; k++)
                {
                    input[k][0] = A[nRow][k];
                    input[k][1] = B[k][nColumn];
                }
                int result;
                MPI_Sendrecv(input, N * 2, MPI_INT, nProcIndex, 0, &result, 1, MPI_INT, nProcIndex, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                C[nRow][nColumn] = result;
                printf("master process starts child# %d for %d,%d\n", nProcIndex, nRow, nColumn);
                nProcIndex++;
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
        int input[N][2];
        MPI_Recv(input, N * 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int Z = 0;
        for (int k = 0; k < N; k++)
        {
            Z += input[k][0] * input[k][1];
        }
        MPI_Send(&Z, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("child process %d replies value=%d\n", rank, Z);
        MPI_Barrier(MPI_COMM_WORLD);

    }
    MPI_Finalize();
}
