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
    int numberOfColumns = B[0].size();
    vector<vector<int>> C(numberOfRows);

    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    int* rows_sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses); // array describing how many row to send to each process
    int* row_displacements = (int*)malloc(sizeof(int) * numberOfProcesses);
    int* columns_sendcounts = (int*)malloc(sizeof(int) * numberOfProcesses); // array describing how many row to send to each process
    int* columns_displacements = (int*)malloc(sizeof(int) * numberOfProcesses);
    int rows_split_size = numberOfRows / numberOfProcesses;
    int columns_split_size = numberOfColumns / numberOfProcesses;
    int rows_remainder = (numberOfRows) % numberOfProcesses; // elements remaining after division among processes
    int columns_remainder = (numberOfColumns) % numberOfProcesses; // elements remaining after division among processes
    int rows_sum = 0;                // Sum of counts. Used to calculate displacements
    int columns_sum = 0;                // Sum of counts. Used to calculate displacements
    for (int i = 0; i < numberOfProcesses; i++) {
        rows_sendcounts[i] = rows_split_size;
        if (rows_remainder > 0) {
            rows_sendcounts[i]++;
            rows_remainder--;
        }
        row_displacements[i] = rows_sum;
        rows_sum += rows_sendcounts[i];

        columns_sendcounts[i] = columns_split_size;
        if (columns_remainder > 0) {
            columns_sendcounts[i]++;
            columns_remainder--;
        }
        columns_displacements[i] = columns_sum;
        columns_sum += columns_sendcounts[i];
    }
    if (rank == 0)
    {
        for (int i = 0; i < numberOfRows; i++)
        {
            vector<int> cells(numberOfColumns);
            C[i] = cells;
        }

    }
    printf("\nrows displcaments:");
    printArray(row_displacements, numberOfProcesses);

    printf("\ncolumns displcaments:");
    printArray(columns_displacements, numberOfProcesses);

    printf("\nrows counts:");
    printArray(rows_sendcounts, numberOfProcesses);

    printf("\ncolumns counts:");
    printArray(columns_sendcounts, numberOfProcesses);

    printf("\nProcess[%d] PartitionA:", rank);
    printf("\n");
    vector<vector<int>> partitionA = Partition(A, 0, numberOfRows, columns_displacements[rank], columns_sendcounts[rank]);
    printMatrix(partitionA);

    printf("\nProcess[%d] PartitionB:", rank);
    vector<vector<int>> partitionB = Partition(B, row_displacements[rank], rows_sendcounts[rank], 0, numberOfColumns);
    printf("\n");
    printMatrix(partitionB);

    int subResultSize = numberOfRows * numberOfColumns;

    if (rank == 0)
    {
        //ROOT PROCESS
        vector<vector<vector<int> > > Intermediate = vector<vector<vector<int> > >(numberOfProcesses, vector<vector<int> >(numberOfRows, vector<int>(numberOfColumns, 0)));
        Intermediate[rank] = SequentialMultiplication(partitionA, partitionB);


        //RECIEVE OTHER PROCESSES INTERMEDIATE MATRICES

        for (int otherProcessIndex = 1; otherProcessIndex < numberOfProcesses; otherProcessIndex++)
        {
            int* otherResultBuffer = (int*)malloc(sizeof(int) * subResultSize);
            MPI_Recv(otherResultBuffer, subResultSize, MPI_INT, otherProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            int index = 0;
            for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
                {
                    Intermediate[otherProcessIndex][rowIndex][columnIndex] = otherResultBuffer[index];
                    index++;
                }
            }
        }

        //int x = 9;
        //MPI_Send(&x, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

        //SUM INTERMEDIATE MATRICES (SEQUENTIAL)
        for (int k = 0; k < numberOfProcesses; k++)
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
        vector<vector<int>> processIntermediateMatrix = SequentialMultiplication(partitionA, partitionB);
        int* subResultBuffer = (int*)malloc(sizeof(int) * subResultSize);
        int bufferIndex = 0;
        for (int rowIndex = 0; rowIndex < numberOfRows; rowIndex++)
        {
            for (int columnIndex = 0; columnIndex < numberOfColumns; columnIndex++)
            {
                subResultBuffer[bufferIndex] = processIntermediateMatrix[rowIndex][columnIndex];
                bufferIndex++;
            }
        }
        MPI_Send(subResultBuffer, subResultSize, MPI_INT, 0, 0, MPI_COMM_WORLD);

        //if (rank == 1)
        //{
        //    int x2 = 0;
        //    MPI_Recv(&x2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        //    printf("GOT X2:%d", x2);
        //}
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

}

vector<vector<int>> IntermediateDataDecomposition::Partition(vector<vector<int>> v, int rowdisplacement, int rows, int columndisplacement, int columns)
{

    vector<vector<int>> partition = vector<vector<int>>(rows, vector<int>(columns));
    printf("\nVector size:[%d X %d] , row displacement:%d , rows count: %d , column displacement: %d , columns count: %d" , rows , columns , rowdisplacement , rows , columndisplacement , columns);
    printf("\n");
    int i = 0;
    int j = 0;
    for (int rowIndex = rowdisplacement, i = 0; i < rows; rowIndex++, i++)
    {
        for (int columnIndex = columndisplacement, j = 0; j < columns; columnIndex++, j++)
        {
            partition[i][j] = v[rowIndex][columnIndex];
        }
    }
    return partition;
}




