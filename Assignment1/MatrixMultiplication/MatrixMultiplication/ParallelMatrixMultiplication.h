#pragma once
#include <iostream>
#include <mpi.h>
#include <vector>
using namespace std;
#define N 3
class ParallelMatrixMultiplication
{
public:
	virtual void Multiply(int A[][N], int B[][N], int C[][N]) = 0;
	virtual void Multiply(vector<vector<int>> A, vector<vector<int>> B) = 0;
protected:
	void printMatrix(vector<vector<int>> A);
	void printArray(int* A, int size);
};

