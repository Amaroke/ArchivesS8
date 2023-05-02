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
	const int N = 100;
	int A[N];
	int B[N];
	int C[2 * N];

	for (int i = 0; i < N; i++) {
		A[i] = i * 2;
	}
	for (int i = 0; i < N; i++) {
		B[i] = i * 2 + 1;
	}

	// Allocate memory for buffers
	cl::Buffer bufferA(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N);
	cl::Buffer bufferB(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N);
	cl::Buffer bufferC(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N * 2);

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferA, true, 0, sizeof(int) * N, A);
	clu_Queue->enqueueWriteBuffer(bufferB, true, 0, sizeof(int) * N, B);
	clu_Queue->enqueueWriteBuffer(bufferB, true, 0, sizeof(int) * N * 2, C);

	// Set up the kernel
	kernel->setArg(0, bufferA);
	kernel->setArg(1, bufferB);
	kernel->setArg(2, bufferC);
	kernel->setArg(3, N);

	cl_int clerr;

    // Launch the kernel
	clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N * 2), cl::NullRange);
		
	// Cheking for errors
	cluCheckError(clerr, "Error running the kernel 1");

	// Read result
	clu_Queue->enqueueReadBuffer(bufferC, true, 0, sizeof(int) * N * 2, C);

	// Print result
	std::cout << "A : ";
	for (int i = 0; i < N; i++)
	{
		std::cout << A[i] << " ";
	}
	std::cout << "+ B : ";
	for (int i = 0; i < N; i++)
	{
		std::cout << B[i] << " ";
	}
	std::cout << "= C : ";
	for (int i = 0; i < N * 2; i++)
	{
		std::cout << C[i] << " ";
	}
}