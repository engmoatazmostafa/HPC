#pragma once
#include "ParallelMatrixMultiplication.h"
class InputDataDecomposition : public ParallelMatrixMultiplication
{

public:
	InputDataDecomposition(int argc, char* argv[]);
	void Multiply(int A[][N], int B[][N], int C[][N]) ;
private:
	int _argc;
	char*** _argv;
};

