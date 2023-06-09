///* merge sort */
//#include <fstream>
//#include <iostream>
//#include <stdio.h>
//#include <mpi.h>
//#include <time.h>
//#include <malloc.h>
//#include <cstdlib>
//#include "SequentialProcessor.h"
////#include <format>
//using namespace std;
////#define N 1000000
//#define N 100
//
////void showElapsed(int id, char* m);
////void showVector(int* v, int n, int id);
//int* merge(int* A, int asize, int* B, int bsize);
////void swap(int* v, int i, int j);
//void mergeSort(int* A, int min, int max);
//
//double startT, stopT;
//
//double startTime;
//
////void showElapsed(int id, char* m)
////{
////	printf("%d: %s %f secs\n", id, m, (clock() - startTime) / CLOCKS_PER_SEC);
////}
////
////void showVector(int* v, int n, int id)
////{
////	int i;
////	printf("%d: ", id);
////	for (i = 0; i < n; i++)
////		printf("%d ", v[i]);
////	putchar('\n');
////}
//
//int* merge(int* A, int asize, int* B, int bsize) {
//	int ai, bi, ci, i;
//	int* C;
//	int csize = asize + bsize;
//
//	ai = 0;
//	bi = 0;
//	ci = 0;
//
//	/* printf("asize=%d bsize=%d\n", asize, bsize); */
//
//	C = (int*)malloc(csize * sizeof(int));
//	while ((ai < asize) && (bi < bsize)) {
//		if (A[ai] <= B[bi]) {
//			C[ci] = A[ai];
//			ci++; ai++;
//		}
//		else {
//			C[ci] = B[bi];
//			ci++; bi++;
//		}
//	}
//
//	if (ai >= asize)
//		for (i = ci; i < csize; i++, bi++)
//			C[i] = B[bi];
//	else if (bi >= bsize)
//		for (i = ci; i < csize; i++, ai++)
//			C[i] = A[ai];
//
//	for (i = 0; i < asize; i++)
//		A[i] = C[i];
//	for (i = 0; i < bsize; i++)
//		B[i] = C[asize + i];
//
//	/* showVector(C, csize, 0); */
//	return C;
//}
//
////void swap(int* v, int i, int j)
////{
////	int t;
////	t = v[i];
////	v[i] = v[j];
////	v[j] = t;
////}
//
//void mergeSort(int* A, int min, int max)
//{
//	int* C;		/* dummy, just to fit the function */
//	int mid = (min + max) / 2;
//	int lowerCount = mid - min + 1;
//	int upperCount = max - mid;
//
//	/* If the range consists of a single element, it's already sorted */
//	if (max == min) {
//		return;
//	}
//	else {
//		/* Otherwise, sort the first half */
//		mergeSort(A, min, mid);
//		/* Now sort the second half */
//		mergeSort(A, mid + 1, max);
//		/* Now merge the two halves */
//		C = merge(A + min, lowerCount, A + mid + 1, upperCount);
//	}
//}
//
//int main(int argc, char** argv)
//{
//	int* data = nullptr;
//	int* chunk;
//	int* other;
//	int m, n = N;
//	int id, p;
//	int s = 0;
//	int i;
//	int step;
//	MPI_Status status;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &id);
//	MPI_Comm_size(MPI_COMM_WORLD, &p);
//
//	startT = clock();
//	SequentialProcessor seqProc;
//	if (id == 0)
//	{
//		int r;
//		srand(clock());
//		s = n / p;
//		r = n % p;
//		int size = 0;
//		if (r != 0)
//		{
//			size = (n + s - r);
//			data = (int*)malloc((n + s - r) * sizeof(int));
//			for (i = 0; i < n; i++)
//				data[i] = rand();
//
//
//			for (i = n; i < n + s - r; i++)
//				data[i] = 0;
//			s = s + 1;
//		}
//		else {
//			size = (n);
//
//			data = (int*)malloc(n * sizeof(int));
//			for (i = 0; i < n; i++)
//				data[i] = rand();
//
//		}
//
//		printf("s: %d,N: %d, size: %d \n", s, N, size);
//		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
//		chunk = (int*)malloc(s * sizeof(int));
//		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
//		//m_sort(chunk, 0, s - 1);
//		seqProc.binarySort(chunk, s);
//		printf("id:%d chuck s:%d\n", id, s);
//
//		 //showVector(chunk, s, id); 
//	}
//	else
//	{
//		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
//		chunk = (int*)malloc(s * sizeof(int));
//		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
//		//m_sort(chunk, 0, s - 1);
//		seqProc.binarySort(chunk, s);
//
//		printf("id:%d chuck s:%d\n", id, s);
//
//		 //showVector(chunk, s, id); 
//	}
//
//	step = 1;
//	while (step < p)
//	{
//		if (id % (2 * step) == 0)
//		{
//			if (id + step < p)
//			{
//				MPI_Recv(&m, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
//				other = (int*)malloc(m * sizeof(int));
//				MPI_Recv(other, m, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
//				chunk = merge(chunk, s, other, m);
//				s = s + m;
//			}
//		}
//		else
//		{
//			int near = id - step;
//			MPI_Send(&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
//			MPI_Send(chunk, s, MPI_INT, near, 0, MPI_COMM_WORLD);
//			break;
//		}
//		step = step * 2;
//	}
//
//	stopT = clock();
//	if (id == 0)
//	{
//		printf("%d; %d processors; %f secs\n", s, p, (stopT - startT) / CLOCKS_PER_SEC);
//
//
//		ofstream logFile;
//		logFile.open("result.txt", ios::app);
//		if (logFile)
//		{
//			for (i = 0; i < s; i++)
//			{
//				logFile << chunk[i];
//				logFile << "\n";
//			}
//			logFile.close();
//		}
//
//
//	}
//	MPI_Finalize();
//}