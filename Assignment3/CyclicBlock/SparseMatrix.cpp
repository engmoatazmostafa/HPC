#include "SparseMatrix.h"
#include "MPIIncludes.h"
SparseMatrix::SparseMatrix(int argc, char* argv[])
{
	_argc = argc;
	_argv = &argv;
}
void SparseMatrix::MultiplyInParallel(vector<vector<int>> A, vector<int> B, vector<int> result)
{
    MPI_Init(&_argc, _argv);
    int rank, numberOfProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    int numberOfRows = A.size();
    int numerOfColumns = B.size(); //number of columns in A = number of rows in B
    int* intermediateResult;
    intermediateResult = (int*)malloc(sizeof(int) * numberOfRows);
    int* assignments = (int*)malloc(sizeof(int) * numberOfProcesses);

    int globalDoneRows = 0;
    int sendCount[1] = { 1 };
    int unassignedRowIndex = -1;
    for (int i = 0; i < numberOfProcesses; i++) {
        assignments[i] = unassignedRowIndex; //unassigned
    }
    if (rank == 0)
    {
        while (globalDoneRows < numberOfRows)
        {
            assignments[0] = globalDoneRows;
            int sum = CalulcateOneValue(A, B, assignments[0]);
            int displacement[1] = { assignments[0] };
            //MPI_Gatherv(&sum, 1, MPI_INT, intermediateResult, sendCount, displacement, MPI_INT, 0, MPI_COMM_WORLD);
            intermediateResult[assignments[0]] = sum;
            //MPI_Gatherv(&unassignedRowIndex, 1, MPI_INT, assignments, sendCount, displacement, MPI_INT, 0, MPI_COMM_WORLD);
            assignments[0] = -1;
            globalDoneRows++;
            printf("\nrow %d done,by process[0] result: d%\n", assignments[0], sum);

            int processAssignmentIndex = globalDoneRows;
            for (int i = 1; i < numberOfProcesses; i++)
            {
                if (assignments[i] == -1 && processAssignmentIndex < numberOfRows)
                {
                    assignments[i] = processAssignmentIndex;
                    MPI_Send(&assignments[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    processAssignmentIndex++;
                    printf("\nrow %d assigned to process: %d\n", assignments[0], i);

                }
            }

            //MPI_Status status;
            //for (int i = 1; i < numberOfProcesses; i++)
            //{
            //    //int flag = 0;
            //    //int test = MPI_Iprobe(i, 1, MPI_COMM_WORLD, &flag, &status);
            //    //if (test == MPI_SUCCESS)
            //    //{
            //        int processResult[2]; //0: result , 1: index in global result
            //        MPI_Recv(&processResult, 2, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            //        int subResult = processResult[0];
            //        int subDisplacement[1] = { processResult[1] };
            //        //MPI_Gatherv(&subResult, 1, MPI_INT, intermediateResult, sendCount, subDisplacement, MPI_INT, 0, MPI_COMM_WORLD);
            //        intermediateResult[processResult[1]] = subResult;
            //        //MPI_Gatherv(&unassignedRowIndex, 1, MPI_INT, assignments, sendCount, subDisplacement, MPI_INT, 0, MPI_COMM_WORLD);
            //        assignments[i] = unassignedRowIndex;
            //        globalDoneRows++;
            //    //}
            //    //else
            //    //{
            //    //    printf("\nMPI_iprobe in process[0] returned:%d\n", test);

            //    //}
            //}
            printf("\nglobalDoneRows:%d\n", globalDoneRows);
        }
        printf("\noutput:");
        for (int i = 0; i < numberOfRows; i++) {
            printf("  %d", intermediateResult[i]);
        }

        printf("\n");
    }
    else
    {
        MPI_Status status;
        int localIndex = 0;
        //int flag = 0;
        //int test = MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, &status);
        //if (test == MPI_SUCCESS)
        //{
            MPI_Recv(&localIndex, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            int sum = CalulcateOneValue(A, B, localIndex);
            int processResult[2] = {sum, localIndex};
            MPI_Send(processResult, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
            printf("\nrow %d done,by process[%d] result: %d\n", localIndex, rank, sum);
        //}
        //else
        //{
        //    printf("\nMPI_iprobe in process[%d] returned:%d\n", rank, test);

        //}

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
