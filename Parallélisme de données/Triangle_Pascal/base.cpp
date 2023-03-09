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
	cl::Kernel* kernel = cluLoadKernel(program, "simple_function");

	// Define the size of the triangle
	int size = 12; // 10 steps

	// Allocate memory for the triangle buffer
	cl::Buffer prev_buffer(*clu_Context, CL_MEM_READ_ONLY, sizeof(int) * size * size);
	cl::Buffer next_buffer(*clu_Context, CL_MEM_WRITE_ONLY, sizeof(int) * size * size);

	// Initialize prev and next with 0, and prev with 1 and 1.
	int* previous_data = new int(size);
	int* next_data = new int(size);

	for (int i = 0; i < size; i++) {
		previous_data[i] = 0;
		next_data[i] = 0;
	}
	previous_data[0] = 1;
	previous_data[1] = 1;

	// Print the first step of the triangle
	for (int i = 0; i < size; ++i) {
		cout << previous_data[i] << " ";
	}
	cout << "\n";

	// Each loop turn is one step of the triangle
	for (int i = 1; i < size - 1; ++i) {

		// Write data inside buffer
		clu_Queue->enqueueWriteBuffer(prev_buffer, true, 0, size * sizeof(int), previous_data);

		// Set up the kernel
		kernel->setArg(0, prev_buffer);
		kernel->setArg(1, next_buffer);

		// Execute the kernel
		clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(i + 2), cl::NDRange(1));

		// Get Data from buffer
		clu_Queue->enqueueReadBuffer(next_buffer, true, 0, size * sizeof(int), next_data);

		//Print this floor of the triangle
		for (int j = 0; j < size; ++j) {
			cout << next_data[j] << " ";
		}
		cout << "\n";

		// Swap next and previous
		previous_data = next_data;
	}

}