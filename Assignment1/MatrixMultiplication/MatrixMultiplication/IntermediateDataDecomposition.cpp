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

    }
    MPI_Finalize();
}

void IntermediateDataDecomposition::Multiply(vector<vector<int>> A, vector<vector<int>> B)
{
    int numberOfRows = A.size();
    int numberOfColumns = A[0].size();
    vector<vector<int>> C(numberOfRows);
    vector<vector<vector<int> > > Intermediate = vector<vector<vector<int> > >(numberOfColumns, vector<vector<int> >(numberOfRows, vector<int>(numberOfColumns, 0)));

    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    int* sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses); // array describing how many row to send to each process
    int* displacements = (int*)malloc(sizeof(int) * numberOfProcesses);

    int minSplitSize = numberOfRows / numberOfProcesses;
    int rem = (numberOfRows) % numberOfProcesses; // elements remaining after division among processes
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
    if (rank == 0)
    {
        for (int i = 0; i < numberOfRows; i++)
        {
            vector<int> cells(numberOfColumns);
            C[i] = cells;
        }

    }


    int lowerIndex = displacements[rank];
    int upperIndex = displacements[rank] + sendcounts[rank] - 1;
    int subResultSize = numberOfRows * numberOfColumns;

    if (rank == 0)
    {
        //ROOT PROCESS
        
        ////CALCULATE INTERMEDIATE MATRICES ASSIGNED TO ROOT
        for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
        {
            for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
            {
                for (int k = lowerIndex; k <= upperIndex; k++)
                {
                    Intermediate[k][rowIndex][columnIndex] = A[rowIndex][k] * B[k][columnIndex];
                }
            }
        }


        ////RECIEVE OTHER PROCESSES INTERMEDIATE MATRICES
        for (int otherProcessIndex = 1; otherProcessIndex < numberOfProcesses; otherProcessIndex++)
        {

            int lowerIndex = displacements[otherProcessIndex];
            int upperIndex = displacements[otherProcessIndex] + sendcounts[otherProcessIndex] - 1;
            for (int k = lowerIndex; k <= upperIndex; k++)
            {
                int* otherResultBuffer = (int*)malloc(sizeof(int) * subResultSize);
                MPI_Recv(otherResultBuffer, subResultSize, MPI_INT, otherProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
                int index = 0;
                for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
                {
                    for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
                    {
                        Intermediate[k][rowIndex][columnIndex] = otherResultBuffer[index];
                        index++;
                    }
                }
            }
        }

        ////SUM INTERMEDIATE MATRICES (SEQUENTIAL)
        for (int k = 0; k < numberOfColumns; k++)
        {
            printf("\nintermediate output[%d]\n" , k);
            printMatrix(Intermediate[k]);

            for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
                {
                    C[rowIndex][columnIndex] += Intermediate[k][rowIndex][columnIndex];
                }
            }
        }

        printf("\nfinal input\n");
        printMatrix(C);
    }
    else
    {
        //NON-ROOT PROCESSES

        ////CALCULATE INTERMEDIATE MATRICES
        for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
        {
            for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
            {
                for (int k = lowerIndex; k <= upperIndex; k++)
                {
                    Intermediate[k][rowIndex][columnIndex] = A[rowIndex][k] * B[k][columnIndex];
                }
            }
        }



        ////SEND TO ROOT 
        for (int k = lowerIndex; k <= upperIndex; k++)
        {


            int* subResultBuffer = (int*)malloc(sizeof(int) * subResultSize);
            int bufferIndex = 0;
            for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
                {
                    subResultBuffer[bufferIndex] = Intermediate[k][rowIndex][columnIndex];
                    bufferIndex++;
                }
            }
            MPI_Send(subResultBuffer, subResultSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }



    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

}
