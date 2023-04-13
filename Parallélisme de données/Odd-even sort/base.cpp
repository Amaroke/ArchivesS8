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
	const char* clu_File = "C:/Users/Amaroke/opencl-base-code-master/base.cl";  // Path to file containing OpenCL kernel(s) code

	cluInit();

	// clu_Context;      <= OpenCL context (pointer)
	// clu_Devices;      <= OpenCL device list (vector)
	// clu_Queue;        <= OpenCL queue (pointer)

	// Load Program
	cl::Program* program = cluLoadProgram(clu_File);
	cl::Kernel* kernel = cluLoadKernel(program, "function");

	// Initialize datas
	const int N = 9;
	int T[N] = { 1, 0, 15, 2, 6, 5, 4, 7, 85 };

	// Allocate memory for buffers
	cl::Buffer bufferT(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N);

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferT, true, 0, sizeof(int) * N, T);

	// Set up the kernel
	kernel->setArg(0, bufferT);

	for (int i = 0; i < N; i++)
	{
			kernel->setArg(1, i);

			// Execute the kernel
			cl_int clerr;
			if (i % 2 == 1) {
				clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N / 2 - 1), cl::NullRange);
			}
			else {
				clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N / 2), cl::NullRange);
			}
			// Checking for errors
			cluCheckError(clerr, "Error running the kernel");
	}

	// Read result
	clu_Queue->enqueueReadBuffer(bufferT, true, 0, sizeof(int) * N, T);

	// Print result
	for (int i = 0; i < N; i++)
	{
		std::cout << T[i] << " ";
	}
}