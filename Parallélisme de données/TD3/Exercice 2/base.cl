int low(__global int* AB, int x, int N);

__kernel void function(__global int* A, __global int* B, __global int* C, int N) {

	int id = get_global_id(0);

	int value;
	int posC;
	if(id > N-1) {
		value = B[id-N];
		posC = low(A, value, N) + id - N;
	} else {
		value = A[id];
		posC = low(B, value, N) + id;
	}

	C[posC] = value;

}

int low(__global int* AB, int x, int N) {
	int cpt = 0;
	int left = 0;
	int right = N - 1;
	
	while (left <= right) {
		int mid = (left + right) / 2;
		
		if (AB[mid] < x) {
			left = mid + 1;
		} else {
			right = mid - 1;
		}
	}
	
	return left;
}

