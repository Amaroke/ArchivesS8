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
	const char* clu_File = "C:/Users/Amaroke/opencl-base-code-master/base.cl";  // Path to file containing OpenCL kernel(s) code

	cluInit();

	// clu_Context;      <= OpenCL context (pointer)
	// clu_Devices;      <= OpenCL device list (vector)
	// clu_Queue;        <= OpenCL queue (pointer)

	// Load Program
	cl::Program* program = cluLoadProgram(clu_File);
	cl::Kernel* kernel = cluLoadKernel(program, "function");

	int N = 3;
	int size = pow(2, N);

	// Allocate memory for the triangle buffer
	cl::Buffer buffer1(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * size);
	cl::Buffer buffer2(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * size);

	// Initialize data
	int data1[8] = { 1, 0, 3, 2, 5, 1, 4, 1 };
	int data2[8];

	// Print first floor
	for (int i = 0; i < size; ++i) {
		cout << data1[i] << " ";
	}
	cout << "\n";

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(buffer1, true, 0, size * sizeof(int), data1);
	clu_Queue->enqueueWriteBuffer(buffer2, true, 0, size * sizeof(int), data2);

	// Each loop turn is one step
	for (int i = 0; i < N; ++i) {

		// Set up the kernel
		if (i % 2 == 0) {
			kernel->setArg(0, buffer1);
			kernel->setArg(1, buffer2);
		}
		else {
			kernel->setArg(1, buffer1);
			kernel->setArg(0, buffer2);
		}
		int offset = pow(2, i);
		kernel->setArg(2, offset);


		// Execute the kernel
		clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(size), cl::NullRange);

		// Get Data from buffer
		if (i % 2 == 0) {
			clu_Queue->enqueueReadBuffer(buffer2, true, 0, size * sizeof(int), data1);
		}
		else {
			clu_Queue->enqueueReadBuffer(buffer1, true, 0, size * sizeof(int), data1);
		}

		// Print this floor
		for (int j = 0; j < size; ++j) {
			cout << data1[j] << " ";
		}
		cout << "\n";
	}

}