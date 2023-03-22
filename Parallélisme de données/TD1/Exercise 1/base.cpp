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
	const int N = 5; // Number of value
	int a[N] = { 2, 6, 8, 15, 23 };
	int croissant[1] = { 1 };

	cl::Buffer buffer(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N);
	cl::Buffer bufferCroissant(*clu_Context, CL_MEM_READ_WRITE, sizeof(int));

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(buffer, true, 0, N * sizeof(int), a);
	clu_Queue->enqueueWriteBuffer(bufferCroissant, true, 0, sizeof(int), croissant);

	kernel->setArg(0, buffer);
	kernel->setArg(1, bufferCroissant);

	// Execute the kernel
	cl_int clerr = clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N - 1), cl::NullRange);

	// Cheking for errors
	cluCheckError(clerr, "Error running the kernel");

	// Reading result
	clu_Queue->enqueueReadBuffer(bufferCroissant, true, 0, sizeof(int), croissant);

	// Print result
	if (croissant[0] == 1) {
		std::cout << "C'est croissant !";
	}
	else {
		std::cout << "Ce n'est pas croissant !";
	}
}