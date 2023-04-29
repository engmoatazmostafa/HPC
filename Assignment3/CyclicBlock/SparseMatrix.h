#pragma once
#include <vector>
using namespace std;
class SparseMatrix
{
public:
	SparseMatrix(int argc, char* argv[]);
	void MultiplyInParallel(vector<vector<int>> A, vector<int> B);
private:
	int _argc;
	char*** _argv;
	int CalulcateOneValue(vector<vector<int>> A, vector<int> B, int localIndex);

};

