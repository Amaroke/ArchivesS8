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
	const int N = 5; // Number of columns/lines
	int T[N * N] = { 5, 3, 0, 3, 4,
					2, 2, 6, 6, 0,
					2, 4, 6, 2, 2,
					1, 1, 2, 3, 3,
					6, 1, 2, 4, 0 };
	int K[N * N] = { 1, 1, 2, 1, 3,
					3, 1, 3, 1, 1,
					1, 1, 3, 2, 3,
					3, 2, 2, 2, 1,
					1, 3, 1, 2, 1 };
	int W[N * N]{};

	// Allocate memory for buffers
	cl::Buffer bufferT(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N * N);
	cl::Buffer bufferK(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N * N);
	cl::Buffer bufferW(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N * N);

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferT, true, 0, sizeof(int) * N * N, T);
	clu_Queue->enqueueWriteBuffer(bufferK, true, 0, sizeof(int) * N * N, K);
	clu_Queue->enqueueWriteBuffer(bufferW, true, 0, sizeof(int) * N * N, W);

	// Set up the kernel
	kernel->setArg(0, bufferT);
	kernel->setArg(1, bufferK);
	kernel->setArg(2, bufferW);
	kernel->setArg(3, N);

	// Execute the kernel
	cl_int clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N * N), cl::NullRange);

	// Cheking for errors
	cluCheckError(clerr, "Error running the kernel");

	// Reading result
	clu_Queue->enqueueReadBuffer(bufferW, true, 0, sizeof(int) * N * N, W);

	// Print result
	for (int i = 0; i < N * N; ++i) {
		std::cout << W[i] << " ";
		if ((i + 1) % N == 0) {
			std::cout << "\n";
		}
	}
}