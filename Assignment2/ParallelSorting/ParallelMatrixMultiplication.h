#pragma once
#define N 3
class ParallelMatrixMultiplication
{
public:
	virtual void Multiply(int A[][N], int B[][N], int C[][N]) = 0;
};

