////#include "testMinMax.h"
////#include <fstream>
////#include <iostream>
////#include <stdio.h>
////#include <mpi.h>
////#include <time.h>
////#include <malloc.h>
////#include <cstdlib>
//#include "ArrayProcessor.h"
////#include "SequentialProcessor.h"
////#include <format>
////using namespace std;
////#define ARRAY_SIZE 100
//#define N 5
//
//int main(int argc, char** argv)
//{
//
//	int *data = (int*)malloc(N * sizeof(int));
//	for (int i = 0; i < N; i++)
//		data[i] = rand();
//	//vector<int> a{ 3, 4, 99, 6, 7 };
//	
//	//int a[ARRAY_SIZE];
//	/*for (int i = 0; i < ARRAY_SIZE; i++) 
//	{
//		a[i] = 14 + i;
//	}*/
//	//printf("%d \n", sizeof(a) / sizeof(int));
//	ArrayProcessor* proc = new ArrayProcessor(argc, argv);
//	//proc->MergeSort(data, N);
//	proc->FindMax(data, N);
//	return 0;
//}