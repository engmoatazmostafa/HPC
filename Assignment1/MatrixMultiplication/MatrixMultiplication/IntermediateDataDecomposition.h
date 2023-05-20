#pragma once
#include "ParallelMatrixMultiplication.h"
class IntermediateDataDecomposition : public ParallelMatrixMultiplication
{

public:
	IntermediateDataDecomposition(int argc, char* argv[]);
	void Multiply(int A[][N], int B[][N], int C[][N]);
	void Multiply(vector<vector<int>> A, vector<vector<int>> B);
private:
	vector<vector<int>> Partition(vector<vector<int>> v, int rowdisplacement, int rows, int columndisplacement, int columns);
	int _argc;
	char*** _argv;
};
