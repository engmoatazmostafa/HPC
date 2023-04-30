#include "ParallelMatrixMultiplication.h"

void ParallelMatrixMultiplication::printMatrix(vector<vector<int>> A)
{
    for (int nRow = 0; nRow < A.size(); nRow++)
    {
        for (int nColumn = 0; nColumn < A[nRow].size(); nColumn++)
        {
            printf(" %d ", A[nRow][nColumn]);
        }
        printf("\n");
    }

}

void ParallelMatrixMultiplication::printArray(int* A, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf(" %d ", A[i]);
    }
}
