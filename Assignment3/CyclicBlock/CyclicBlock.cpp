#include <iostream>
#include <vector>
#include "SparseMatrix.h"
using namespace std;

void MultiplySequential(vector<vector<int>> A, vector<int> B)
{
    vector<int> output(B.size());
    for (int row = 0; row < A.size(); row++)
    {
        int sum = 0;
        for (int i = 0; i < B.size(); i++)
        {
            sum += A[row][i] * B[i];
        }
        output[row] = sum;
    }
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
    vector<int> result;


    matrixProcesser->MultiplyInParallel(A, B);
    return 0;

}
