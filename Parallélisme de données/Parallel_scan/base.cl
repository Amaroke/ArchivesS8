__kernel void function(__global const int* prev, __global int* next, int offset) {
	if(get_global_id(0) < offset) {
		next[get_global_id(0)] = prev[get_global_id(0)];
		return;
	}
 	next[get_global_id(0)] = prev[get_global_id(0)] + prev[get_global_id(0) - offset];
	return;
}