__kernel void function(__global const int* T, __global const int* K, __global int* W, int N) {
	int id = get_global_id(0);
	int k_value = K[id];
	int r = id / N;
	int c = id % N;
	int maxi = max(0, r - k_value); 
	int mini = min(N - 1, r + k_value);
	int compute = 0;
	for(int i = maxi; i <= mini; ++i) {
		compute += T[i*N + c];
	}
	W[get_global_id(0)] = compute;
}