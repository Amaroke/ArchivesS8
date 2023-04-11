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
	const int K = 1000;
	int N = K * K;
	int M[1] = { 0 };

	// Allocate memory for buffers
	cl::Buffer bufferM(*clu_Context, CL_MEM_READ_WRITE, sizeof(int));


	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferM, true, 0, sizeof(int), M);

	// Set up the kernel
	kernel->setArg(0, bufferM);
	kernel->setArg(1, K);
	kernel->setArg(2, cl::__local((K+K) * sizeof(float)));

	// Execute the kernel
	cl_int clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N), cl::NDRange(K));

	// Cheking for errors
	cluCheckError(clerr, "Error running the kernel");

	// Read result
	clu_Queue->enqueueReadBuffer(bufferM, true, 0, sizeof(int), M);

	// Print result
	std::cout << 4.0*M[0]/N << " ";

}