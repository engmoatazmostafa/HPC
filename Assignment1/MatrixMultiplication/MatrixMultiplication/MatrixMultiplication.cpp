#include <iostream>
#include "ParallelMatrixMultiplicationFactory.h"
vector<vector<int>> CreateMatrix(int dimensionSize, bool randomNumbers)
{
    vector<vector<int>> v(dimensionSize, vector<int>(dimensionSize, 1));
    if (!randomNumbers) return v;

    for (int i = 0; i < dimensionSize; i++)
    {
        for (int j = 0; j < dimensionSize; j++)
        {
            v[i][j] = rand() % 10 + 1;
        }
    }
    return v;
}
int main(int argc, char* argv[])
{
    bool randomNumbers = false;
    int dimensionSize = 4;
    DataDecomposition dataDecompisition = DataDecomposition::Input;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-dc") == 0 && i < argc - 1)
        {
            if (strcmp(argv[i + 1], "1") == 0)
            {
                dataDecompisition = DataDecomposition::Input;
            }
            else if (strcmp(argv[i + 1], "2") == 0)
            {
                dataDecompisition = DataDecomposition::Output;
            }
            else if (strcmp(argv[i + 1], "3") == 0)
            {
                dataDecompisition = DataDecomposition::Intermediate;
            }
        }
        else if (strcmp(argv[i], "-s") == 0 && i < argc - 1)
        {
            try
            {
                dimensionSize = atoi(argv[i + 1]);
            }
            catch (exception e)
            {

            }
        }
        else if (strcmp(argv[i], "-r") == 0 && i < argc - 1)
        {
            try
            {
                randomNumbers = atoi(argv[i + 1]) == 1 ? true : false;
            }
            catch (exception e)
            {

            }
        }
    }

    vector<vector<int>> A = CreateMatrix(dimensionSize, randomNumbers);
    vector<vector<int>> B = CreateMatrix(dimensionSize, randomNumbers);

    ParallelMatrixMultiplication* instance = ParallelMatrixMultiplicationFactory::CreateInstance(dataDecompisition,argc, argv);

    instance->Multiply(A, B);

    return 0;

}
