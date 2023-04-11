__kernel void function(__global int* T, __local int* local_table, __global int* sum) {
    int id = get_global_id(0);

	// Prefetch
    local_table[get_local_id(0)] = T[get_global_id(0)];
    barrier(CLK_LOCAL_MEM_FENCE);
    if (get_local_id(0) == 0) {
        int local_sum = 0;
        for (int i = 0; i < get_local_size(0); i++) {
            local_sum += local_table[i];
        }
        atomic_add(&sum[0], local_sum);
    }
}