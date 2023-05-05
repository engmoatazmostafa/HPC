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

void InputDataDecomposition::Multiply(vector<vector<int>> A, vector<vector<int>> B)
{
    int numberOfRows = A.size();
    int numberOfColumns = A[0].size();
    vector<vector<int>> C(numberOfRows);
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    // array describing how many rows to send to each process
    int* sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses); 
    // array describing how many rows to skip to each process
    int* displacements = (int*)malloc(sizeof(int) * numberOfProcesses);

    int minSplitSize = numberOfRows / numberOfProcesses;
    // elements remaining after division among processes
    int rem = (numberOfRows) % numberOfProcesses; 
    // Sum of counts. Used to calculate displacements
    int sum = 0;                
    for (int i = 0; i < numberOfProcesses; i++) {
        sendcounts[i] = minSplitSize;
        if (rem > 0) {
            sendcounts[i]++;
            rem--;
        }
        displacements[i] = sum;
        sum += sendcounts[i];
    }

    if (rank == 0)
    {
        for (int i = 0; i < numberOfRows; i++)
        {
            vector<int> cells(numberOfColumns);
            C[i] = cells;
        }
    }
    
    //rows indeces range for current process
    int lowerRowIndex = displacements[rank];
    int upperRowIndex = displacements[rank] + sendcounts[rank] - 1;

    if (rank == 0)
    {
        //root process

        //calculate output for assigned rows
        for (int rowIndex = lowerRowIndex; rowIndex <= upperRowIndex; rowIndex++)
        {
            for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
            {
                int cellValue = 0;
                for (int k = 0; k < numberOfColumns; k++)
                {
                    cellValue += A[rowIndex][k] * B[k][columnIndex];
                }
                printf("Process[%d], row Index:%d\n", rank, rowIndex);
                C[rowIndex][columnIndex] = cellValue;
            }
        }

        //sequential part (other process will not do it):  merge output for other processes
        for (int otherProcessIndex = 1; otherProcessIndex < numberOfProcesses; otherProcessIndex++)
        {
            int otherResultSize = sendcounts[otherProcessIndex] * numberOfColumns;
            int* otherResultBuffer = (int*)malloc(sizeof(int) * otherResultSize);
            MPI_Recv(otherResultBuffer, otherResultSize, MPI_INT, otherProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            int index = 0;
            while (index < otherResultSize)
            {
                int rowIndex = (index / numberOfColumns) + displacements[otherProcessIndex];
                int columnIndex = index % numberOfColumns;
                C[rowIndex][columnIndex] = otherResultBuffer[index];
                index++;
            }
        }

        printf("\nfinal output, input data decomposition\n");
        printMatrix(C);
    }
    else
    {
        //child process
        
        //create buffer for output
        int subResultSize = sendcounts[rank] * numberOfColumns;
        int* subResultBuffer = (int*)malloc(sizeof(int) * subResultSize);
        int bufferIndex = 0;
        
        //calculate output for assigned rows
        for (int rowIndex = lowerRowIndex; rowIndex <= upperRowIndex; rowIndex++)
        {
            for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
            {
                int cellValue = 0;
                for (int k = 0; k < numberOfColumns; k++)
                {
                    cellValue += A[rowIndex][k] * B[k][columnIndex];
                }
                printf("Process[%d], row Index:%d\n", rank, rowIndex);
                subResultBuffer[bufferIndex] = cellValue;
                bufferIndex++;
            }
        }

        //send output to root process
        MPI_Send(subResultBuffer, subResultSize, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
}
