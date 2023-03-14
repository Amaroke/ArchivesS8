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

	// Initialize datas
	const int N = 7;
	const int K = 3;
	int A[N] = { 1, 1, 0, 1, 2, 1, 0 };
	int R[N * 2];
	int H[K] = { -1, -1, -1 };

	cl::Buffer bufferA(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N);
	cl::Buffer bufferR(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * N * 2);
	cl::Buffer bufferH(*clu_Context, CL_MEM_READ_WRITE, sizeof(int) * K);

	// Write data inside buffer
	clu_Queue->enqueueWriteBuffer(bufferA, true, 0, N * sizeof(int), A);
	clu_Queue->enqueueWriteBuffer(bufferH, true, 0, K * sizeof(int), H);

	kernel->setArg(0, bufferA);
	kernel->setArg(1, bufferR);
	kernel->setArg(2, bufferH);	
	cl::Event ev;
	// Execute the kernel
	clu_Queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(N), cl::NullRange, NULL, &ev);
	
	// Compute time
	ev.wait();
	cluDisplayEventMilliseconds("kernel took ", ev);

	// Read the kernel
	clu_Queue->enqueueReadBuffer(bufferR, true, 0, N * 2 * sizeof(int), R);
	clu_Queue->enqueueReadBuffer(bufferH, true, 0, K * sizeof(int), H);

	// Print R
	for (int i = 0; i < N*2; ++i) {
		cout << R[i] << " ";
	}
	cout << "\n";

	// Print H
	for (int i = 0; i < K; ++i) {
		cout << H[i] << " ";
	}
	cout << "\n";
}