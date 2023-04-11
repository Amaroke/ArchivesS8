__kernel void function(__global int* M, __local int* local_table, __global int* distance_minimale, int n) {
   int id = get_global_id(0);
    int r = id / n;
	int c = id % n;
    if(r != c) {
        int Ax = M[r];
        int Ay = M[c];
        int Bx = M[r + n];
        int By = M[c + n];

        local_table[get_global_id(0)] = (Ax-Ay)*(Ax-Ay) + (Bx-By)*(Bx-By);
        
    } else {
        local_table[get_global_id(0)] = distance_minimale;
    }

    barrier(CLK_LOCAL_MEM_FENCE);
    if (id == 1) {        
        for (int i = 0; i < n*n; i++) {
            atomic_min(distance_minimale, local_table[i]);
        }
    }
    
}