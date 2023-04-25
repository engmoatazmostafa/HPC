#pragma once
#include "ParallelMatrixMultiplication.h"
enum DataDecomposition
{
	Input,
	Intermediate,
	Output
};
class ParallelMatrixMultiplicationFactory
{
public:
	static ParallelMatrixMultiplication* CreateInstance(DataDecomposition dataDecompisition, int argc, char* argv[]);
};

