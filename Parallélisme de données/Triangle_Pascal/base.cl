__kernel void simple_function(__global const int* prev, __global int* next) {
    // Calculate the value
    next[get_global_id(0)] = prev[get_global_id(0)] + prev[get_global_id(0) - 1];
    return;
}