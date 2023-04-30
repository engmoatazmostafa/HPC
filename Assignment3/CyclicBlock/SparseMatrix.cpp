#include "SparseMatrix.h"
#include "MPIIncludes.h"
SparseMatrix::SparseMatrix(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;
}
/// <summary>
/// multiply sparse matrix by a vector
/// parallel execution 
/// task decomposed by round-robin method to get maximum load balancing
/// </summary>
/// <param name="A">sparse matrix</param>
/// <param name="B">vector</param>
void SparseMatrix::MultiplyInParallel(vector<vector<int>> A, vector<int> B)
{
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    int numberOfRows = A.size();
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
    int chunkSize = 0;
    //round-robin rows data partitioning 
    while (nRowIndex < numberOfRows)
    {
        int sum = CalulcateOneValue(A, B, nRowIndex);
        printf("\nrow[%d] in process[%d] result:%d\n", nRowIndex, rank, sum);
        resultChunk[chunkSize] = sum;
        assignments[chunkSize] = nRowIndex;
        nRowIndex += numberOfProcesses;
        chunkSize++;
    }
    if (rank == 0)
    {
        for (int i = 0; i < chunkSize; i++)
        {
            intermediateResult[assignments[i]] = resultChunk[i];
        }
        for (int nProcessIndex = 1; nProcessIndex < numberOfProcesses; nProcessIndex++)
        {
            //get result from others
            int tmpChuckSize;
            int* tmpAssignments = (int*)malloc(sizeof(int) * sizeOfProcessChunk);
            int* tmpResults = (int*)malloc(sizeof(int) * sizeOfProcessChunk);
            MPI_Recv(&tmpChuckSize, 1, MPI_INT, nProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            MPI_Recv(tmpAssignments, tmpChuckSize, MPI_INT, nProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            MPI_Recv(tmpResults, tmpChuckSize, MPI_INT, nProcessIndex, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            //set the global array 
            for (int i = 0; i < tmpChuckSize; i++)
            {
                intermediateResult[tmpAssignments[i]] = tmpResults[i];
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
        //send result to root
        MPI_Send(&chunkSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(assignments, chunkSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(resultChunk, chunkSize, MPI_INT, 0, 0, MPI_COMM_WORLD);
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
