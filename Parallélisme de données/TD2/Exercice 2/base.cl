__kernel void function(__global int* M, int K, __local float* local_table) {
    int id = get_global_id(0);
    float point[2] = { get_local_id(0)/convert_float(K)-0.5, (((get_global_id(0)/K)/convert_float(K))-0.5)};

	// Prefetch
    local_table[get_local_id(0)] = point[0];
    local_table[get_local_id(0)+K] = point[1];
    barrier(CLK_LOCAL_MEM_FENCE);
    if (get_local_id(0) == 0) {
        int inside_circle = 0;        
        for (int i = 0; i < get_local_size(0); i++) {
            if(sqrt(local_table[i]*local_table[i] + local_table[i+K]*local_table[i+K]) < 0.5) {
                inside_circle += 1;
            }
        }
        atomic_add(&M[0], inside_circle);
    }
}