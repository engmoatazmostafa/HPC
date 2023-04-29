#include <iostream>
#include <vector>
#include "SparseMatrix.h"
using namespace std;
void sequentialMultiplication(vector<vector<int>> A, vector<int> B, vector<int> result) {
    for (int rowIndex = 0; rowIndex < B.size(); rowIndex++)
    {
        int sum = 0;
        int sizeOfRow = A[rowIndex].size();
        for (int i = 0; i < B.size() && i < sizeOfRow; i++)
        {
            if (A[rowIndex][i] != 0 && B[i] != 0)
            {
                sum += A[rowIndex][i] * B[i];
            }
        }
        result.push_back(sum);
        //result[rowIndex] = sum;
    }
    printf("\nsequential output:");
    for (int i = 0; i < result.size(); i++) {
        printf("  %d", result[i]);
    }

    printf("\n");
}
int main(int argc, char* argv[])
{
    SparseMatrix* matrixProcesser = new SparseMatrix(argc, argv);
    vector<vector<int>> A = {
         {1,0,0,1,1}
        ,{0,0,0,1,1}
        ,{0,1,0,1,1}
        ,{0,0,0,0,1}
        ,{0,0,0,1,1}
    };
    vector<int> B = { 1,0,0,1,1 };
    vector<int> result = { 0,0,0,0,0 };

    sequentialMultiplication(A, B, result);

    matrixProcesser->MultiplyInParallel(A, B, result);
    return 0;

}
