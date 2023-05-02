__kernel void function(__global int* T, int N, __global int* modif, __global int* triRow) {

    int id = get_global_id(0);
	int r = id / N;
	int c = id % N;

    if(triRow[0] == 0) {
		int i = id*N;
		for(int j = i; j<i+N-1; j++) {
			if(id%2 == 0) {
				if(T[j] > T[j+1]) {
					int temp = T[j];
					T[j] = T[j+1];
					T[j+1] = temp;
					atomic_xchg(modif, 1);
				}
			} else {
				if(T[j] < T[j+1]) {
					int temp = T[j];
					T[j] = T[j+1];
					T[j+1] = temp;
					atomic_xchg(modif, 1);
				}
			}
		}
    } else {
		for(int j = id; j<N*N-N; j += N) {
			if(T[j] > T[j+N]) {
				int temp = T[j];
				T[j] = T[j+N];
				T[j+N] = temp;
				atomic_xchg(modif, 1);
			}
		}
    }
}


