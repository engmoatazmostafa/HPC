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
vector<vector<int>> ParallelMatrixMultiplication::SequentialMultiplication(vector<vector<int>> A, vector<vector<int>> B)
{
    int rows = A.size();
    int firstMatrixColumns = A[0].size();
    int columns = B[0].size();
    int secondMatrixRows = B.size();
    vector<vector<int>> result = vector<vector<int>>(rows, vector<int>(columns, 0));
    if (firstMatrixColumns != secondMatrixRows) return result;

    for (int rowIndex = 0; rowIndex < rows; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < columns; columnIndex++)
        {
            for (int k = 0; k < firstMatrixColumns; k++)
            {
                result[rowIndex][columnIndex] += A[rowIndex][k] * B[k][columnIndex];
            }
        }
    }

    return result;
}