#include "SparseMatrix.h"
#include "MPIIncludes.h"
SparseMatrix::SparseMatrix(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;
}
void SparseMatrix::MultiplyInParallel(vector<vector<int>> A, vector<int> B)
{
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    int numberOfRows = A.size();
    int numerOfColumns = B.size(); //number of columns in A = number of rows in B
    int* intermediateResult;
    intermediateResult = (int*)malloc(sizeof(int) * numberOfRows);
    int sizeOfProcessChunk = numberOfRows / numberOfProcesses;
    int rem = numberOfRows % numberOfProcesses;
    if (rem > 0) {
        sizeOfProcessChunk++;
    }
    int* assignments = (int*)malloc(sizeof(int) * sizeOfProcessChunk);
    int* resultChunk = (int*)malloc(sizeof(int) * sizeOfProcessChunk);

    int nRowIndex = rank;
    int count = 0;
    while (nRowIndex < numberOfRows)
    {
        int sum = CalulcateOneValue(A, B, nRowIndex);
        printf("\nrow[%d] in process[%d] result:%d\n", nRowIndex, rank, sum);
        resultChunk[count] = sum;
        assignments[count] = nRowIndex;
        nRowIndex += numberOfProcesses;
        count++;
    }
    while (count < sizeOfProcessChunk)
    {
        assignments[count] = -1;
        resultChunk[count] = 0;
        count++;
    }
    if (rank == 0)
    {
        for (int i = 0; i < count; i++)
        {
            if (assignments[i] > -1)
            {

                intermediateResult[assignments[i]] = resultChunk[i];
            }
        }
        for (int nProcessIndex = 1; nProcessIndex < numberOfProcesses; nProcessIndex++)
        {
            int* tmpAssignments = (int*)malloc(sizeof(int) * sizeOfProcessChunk);
            int* tmpResults = (int*)malloc(sizeof(int) * sizeOfProcessChunk);
            MPI_Recv(tmpAssignments, sizeOfProcessChunk, MPI_INT, nProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            MPI_Recv(tmpResults, sizeOfProcessChunk, MPI_INT, nProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            for (int i = 0; i < sizeOfProcessChunk; i++)
            {
                if (tmpAssignments[i] > -1) 
                {
                    intermediateResult[tmpAssignments[i]] = tmpResults[i];
                }
            }
        }
        printf("\noutput:");
        for (int i = 0; i < numberOfRows; i++) {
            printf("  %d", intermediateResult[i]);
        }

        printf("\n");

    }
    else 
    {
        MPI_Send(assignments, sizeOfProcessChunk, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(resultChunk, sizeOfProcessChunk, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

int SparseMatrix::CalulcateOneValue(vector<vector<int>> A, vector<int> B, int localIndex)
{
    int sum = 0;
    int sizeOfRow = A[localIndex].size();
    for (int i = 0; i < B.size() && i < sizeOfRow; i++)
    {
        if (A[localIndex][i] != 0 && B[i] != 0)
        {
            sum += A[localIndex][i] * B[i];
        }
    }
    return sum;
}   
