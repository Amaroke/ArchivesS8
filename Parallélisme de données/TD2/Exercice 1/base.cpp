#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include <chrono>

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
	const int N = 16; // Nombre d'entiers du tableau
	const int G = 4; // Nombre de groupes
	int T[N] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	int sum[1] = { 0 };

	// Allocate memory for buffers
	cl::Buffer bufferT(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N);
	cl::Buffer bufferSum(*clu_Context, CL_MEM_READ_WRITE, sizeof(int));


	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferT, true, 0, sizeof(int) * N, T);
	clu_Queue->enqueueWriteBuffer(bufferSum, true, 0, sizeof(int), sum);


	// Set up the kernel
	kernel->setArg(0, bufferT);
	kernel->setArg(1, cl::__local((G) * sizeof(int)));
	kernel->setArg(2, bufferSum);

	// Execute the kernel
	cl_int clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N), cl::NDRange(G));

	// Cheking for errors
	cluCheckError(clerr, "Error running the kernel");

	// Read result
	clu_Queue->enqueueReadBuffer(bufferSum, true, 0, sizeof(int), sum);

	// Print result
	std::cout << sum[0] << " ";

}