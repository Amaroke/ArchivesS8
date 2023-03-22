__kernel void function(__global const int* a, __global int* croissant) {

	if (a[get_global_id(0)] >= a[get_global_id(0) + 1] && croissant[0] == 1) {
		int nothing = atomic_xchg(&croissant[0], 0);
	}

}