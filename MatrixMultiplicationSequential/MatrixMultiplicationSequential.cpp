// MatrixMultiplicationSequential.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

#define N 3

// This function multiplies
// mat1[][] and mat2[][], and
// stores the result in res[][]
void multiply(int mat1[][N],
    int mat2[][N],
    int res[][N])
{
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            res[i][j] = 0;
            for (k = 0; k < N; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}

// Driver Code
int main()
{
    int i, j;
    int res[N][N]; // To store result
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

    multiply(A, B, res);

    cout << "Result matrix is " << endl;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            cout << res[i][j] << " ";
        cout << endl;
    }

    return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
