__kernel void function(__global const int* A, __global int* R, __global int* H) {

	R[get_global_id(0) * 2] = get_global_id(0);
	R[get_global_id(0) * 2 + 1] = atomic_xchg(&H[A[get_global_id(0)]], get_global_id(0) * 2);

}