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
	const int N = 4;
	int T[N*N] = { 13, 1, 6, 5, 16, 8, 3, 12, 7, 11, 4, 2, 10, 15, 9, 14 };
	int modif[1] = { 0 };
	int triRow[1] = { 0 }; // 0 = Raw 1 = col
	int temp[1] = { 0 };


	// Allocate memory for buffers
	cl::Buffer bufferT(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N * N);
	cl::Buffer bufferModif(*clu_Context, CL_MEM_READ_WRITE, sizeof(int));
	cl::Buffer bufferTriRow(*clu_Context, CL_MEM_READ_WRITE, sizeof(int));

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferT, true, 0, sizeof(int) * N * N, T);
	clu_Queue->enqueueWriteBuffer(bufferModif, true, 0, sizeof(int), modif);
	clu_Queue->enqueueWriteBuffer(bufferTriRow, true, 0, sizeof(int), triRow);

	// Set up the kernel
	kernel->setArg(0, bufferT);
	kernel->setArg(1, N);
	kernel->setArg(2, bufferModif);
	kernel->setArg(3, bufferTriRow);

	cl_int clerr;

	modif[0] = 1;
	while(modif[0] != 0)
	{
		modif[0] = 0;
		clu_Queue->enqueueWriteBuffer(bufferModif, true, 0, sizeof(int), modif);

		triRow[0] = 0;
		clu_Queue->enqueueWriteBuffer(bufferTriRow, true, 0, sizeof(int), triRow);

        // Launch the kernel
		clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N), cl::NullRange);
		
		// Cheking for errors
		cluCheckError(clerr, "Error running the kernel 1");

		triRow[0] = 1;
		clu_Queue->enqueueWriteBuffer(bufferTriRow, true, 0, sizeof(int), triRow);

		clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N), cl::NullRange);
		
		// Cheking for errors
		cluCheckError(clerr, "Error running the kernel 2");	
		
		// Read result (test modification)
		clu_Queue->enqueueReadBuffer(bufferModif, true, 0, sizeof(int), modif);

	}

	// Read result
	clu_Queue->enqueueReadBuffer(bufferT, true, 0, sizeof(int) * N * N, T);

	// Print result
	for (int i = 0; i < N * N; i++)
	{
		if (i % N == 0) {
			std::cout << "\n";
		}
		std::cout << T[i] << " ";
	}
}