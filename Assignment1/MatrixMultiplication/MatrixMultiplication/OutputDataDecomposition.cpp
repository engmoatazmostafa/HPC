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

    }
    MPI_Finalize();
}

void OutputDataDecomposition::Multiply(vector<vector<int>> A, vector<vector<int>> B)
{
    int numberOfRows = A.size();
    int numberOfColumns = A[0].size();
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    int* sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses); // array describing how many row to send to each process
    int* displacements = (int*)malloc(sizeof(int) * numberOfProcesses);

    int minSplitSize = (numberOfRows * numberOfColumns) / numberOfProcesses;
    int rem = (numberOfRows * numberOfColumns) % numberOfProcesses; // elements remaining after division among processes
    int sum = 0;                // Sum of counts. Used to calculate displacements
    for (int i = 0; i < numberOfProcesses; i++) {
        sendcounts[i] = minSplitSize;
        if (rem > 0) {
            sendcounts[i]++;
            rem--;
        }
        displacements[i] = sum;
        sum += sendcounts[i];
    }


    int lowerRowIndex = displacements[rank];
    int upperRowIndex = displacements[rank] + sendcounts[rank] - 1;
    if (rank == 0)
    {
        vector<vector<int>> C(numberOfRows);
        for (int i = 0; i < numberOfRows; i++)
        {
            vector<int> cells(numberOfColumns);
            C[i] = cells;
        }

        for (int index = lowerRowIndex; index <= upperRowIndex; index++)
        {
            int rowIndex = index / numberOfColumns;
            int columnIndex = index % numberOfColumns;
            int cellValue = 0;
            printf("\n process[%d],index:%d, row:%d, column:%d" , rank, index, rowIndex, columnIndex);
            for (int k = 0; k < numberOfColumns; k++)
            {
                cellValue += A[rowIndex][k] * B[k][columnIndex];
            }
            C[rowIndex][columnIndex] = cellValue;
        }
        for (int otherProcessIndex = 1; otherProcessIndex < numberOfProcesses; otherProcessIndex++)
        {
            int otherResultSize = sendcounts[otherProcessIndex];
            int* otherResultBuffer = (int*)malloc(sizeof(int) * otherResultSize);
            MPI_Recv(otherResultBuffer, otherResultSize, MPI_INT, otherProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

            int lowerIndex = displacements[otherProcessIndex];
            int upperIndex = displacements[otherProcessIndex] + sendcounts[otherProcessIndex] - 1;
            int bufferIndex = 0;
            for (int index = lowerIndex; index <= upperIndex; index++, bufferIndex++)
            {
                int rowIndex = index / numberOfColumns;
                int columnIndex = index % numberOfColumns;
                C[rowIndex][columnIndex] = otherResultBuffer[bufferIndex];
            }

        }

        printf("\nfinal output, output decompisition\n");
        printMatrix(C);
    }
    else
    {
        int subResultSize = sendcounts[rank];
        int* subResultBuffer = (int*)malloc(sizeof(int) * subResultSize);
        int bufferIndex = 0;
        for (int index = lowerRowIndex; index <= upperRowIndex; index++, bufferIndex++)
        {
            int rowIndex = index / numberOfColumns;
            int columnIndex = index % numberOfColumns;
            int cellValue = 0;
            printf("\n process[%d],index:%d, row:%d, column:%d", rank, index, rowIndex, columnIndex);

            for (int k = 0; k < numberOfColumns; k++)
            {
                cellValue += A[rowIndex][k] * B[k][columnIndex];
            }
            subResultBuffer[bufferIndex] = cellValue;
        }
        MPI_Send(subResultBuffer, subResultSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
}
