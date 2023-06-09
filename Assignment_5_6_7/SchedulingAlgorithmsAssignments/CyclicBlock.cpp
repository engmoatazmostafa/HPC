#include <iostream>
#include <vector>
#include "SparseMatrix.h"
using namespace std;

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

    matrixProcesser->MultiplyInParallel(A, B, result);
    return 0;

}
