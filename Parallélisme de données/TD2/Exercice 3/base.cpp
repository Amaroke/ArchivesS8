#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <chrono>
#include <time.h>

using namespace std;

#include "clutils.h"

int main(int argc, char** argv)
{
	const char* clu_File = "base.cl";  // Path to file containing OpenCL kernel(s) code

	cluInit();

	// clu_Context;      <= OpenCL context (pointer)
	// clu_Devices;      <= OpenCL device list (vector)
	// clu_Queue;        <= OpenCL queue (pointer)

	// Load Program
	cl::Program* program = cluLoadProgram(clu_File);
	cl::Kernel* kernel = cluLoadKernel(program, "function");

	// Initialize datas
	const int n = 4;
	int M[2*n] = {	6, 1, 4, 13,
					4, 9, 6, 12 };
	float distance_minimale = FLT_MAX;
	int distance_minimale_kernel[1] = { INT_MAX };
	
	clock_t begin = clock();

	// Algorithme séquentiel 
	for (int i = 0; i < n; ++i) {
		for (int j = i+1; j < n; ++j) {
			float distance = sqrt((M[i] - M[j]) * (M[i] - M[j]) + (M[i + n] - M[j + n]) * (M[i + n] - M[j + n]));
			if ( distance < distance_minimale) {
				distance_minimale = distance;
			}
		}
	}

	clock_t end = clock();

	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	std::cout << "Distance minimale en sequentiel : " << distance_minimale << "\n";
	printf("Temps d'execution : %f\n", time_spent);

	// Allocate memory for buffers
	cl::Buffer bufferM(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * n * n);
	cl::Buffer bufferDist(*clu_Context, CL_MEM_READ_WRITE, sizeof(int));

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferM, true, 0, sizeof(int) * n * n, M);
	clu_Queue->enqueueWriteBuffer(bufferDist, true, 0, sizeof(int), distance_minimale_kernel);


	// Set up the kernel
	kernel->setArg(0, bufferM);
	kernel->setArg(1, cl::__local((n) * sizeof(int)));
	kernel->setArg(2, bufferDist);
	kernel->setArg(3, n);

	// Execute the kernel
	cl_int clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(n*n), cl::NullRange);

	// Cheking for errors
	cluCheckError(clerr, "Error running the kernel");

	// Read result
	clu_Queue->enqueueReadBuffer(bufferDist, true, 0, sizeof(int), distance_minimale_kernel);

	// Print result
	std::cout << "Distance minimale avec OpenCL : sqrt(" << distance_minimale_kernel[0] << ") = " << sqrt(distance_minimale_kernel[0]);
}