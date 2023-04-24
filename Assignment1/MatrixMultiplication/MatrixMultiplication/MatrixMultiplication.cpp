#include <iostream>
#include "ParallelMatrixMultiplicationFactory.h"
int main(int argc, char* argv[])
{
    
    int A[N][N] = { 
        { 1, 1, 1 },
        { 2, 2, 2 },
        { 3, 3, 3 } };

    int B[N][N] = { 
        { 1, 1, 1 },
        { 2, 2, 2 },
        { 3, 3, 3 } };

    int C[N][N] = { 
            { 0, 0 , 0},
            { 0, 0 , 0},
            { 0, 0 , 0} };

    DataDecomposition dataDecompisition = DataDecomposition::Input;
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]).compare("data-decomposition-input"))
        {
            dataDecompisition = DataDecomposition::Input;
            break;
        }
        else if (std::string(argv[i]).compare("data-decomposition-output"))
        {
            dataDecompisition = DataDecomposition::Output;
            break;
        }
        else if (std::string(argv[i]).compare("data-decomposition-intermediate"))
        {
            dataDecompisition = DataDecomposition::Intermediate;
            break;
        }
    }

    ParallelMatrixMultiplication* instance = ParallelMatrixMultiplicationFactory::CreateInstance(dataDecompisition,argc, argv);
    instance->Multiply(A, B, C);

    return 0;

}
