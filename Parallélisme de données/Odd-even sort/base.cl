__kernel void function(__global int* T, int i) {

    int id = get_global_id(0);
    int indice = id*2;

    if(T[indice+i%2] > T[indice+i%2+1]) {
        int tmp = T[indice+i%2];
        T[indice+i%2] = T[indice+i%2+1];
        T[indice+i%2+1] = tmp;
    }
    
}