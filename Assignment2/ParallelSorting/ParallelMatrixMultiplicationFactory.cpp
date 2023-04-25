#include "ParallelMatrixMultiplicationFactory.h"
#include "InputDataDecomposition.h"
#include "OutputDataDecomposition.h"
#include "IntermediateDataDecomposition.h"
ParallelMatrixMultiplication* ParallelMatrixMultiplicationFactory::CreateInstance(DataDecomposition dataDecompisition, int argc, char* argv[])
{
	if (dataDecompisition == DataDecomposition::Input) 
	{
		return new InputDataDecomposition(argc, argv);
	}
	else if (dataDecompisition == DataDecomposition::Intermediate)
	{
		return new IntermediateDataDecomposition(argc, argv);
	}
	else 
	{
		return new OutputDataDecomposition(argc, argv);
	}
}
