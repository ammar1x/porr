

__kernel void solve(__global int* instances,
                    __global int* objectsValues,
                    __global int* objectsWeights,
                     __global int* out, int n, int capacity)
{
    int id = get_global_id(0);
    __global int* start = &instances[id*n];
    __global int* insertObject = start;

    int value = 0;
    int totalWeight = 0;


    for(int i = 0; i < n; ++i) {

        if (insertObject[i] == 1) {
           value += objectsValues[i];
           totalWeight += objectsWeights[i];
        }
    }
    if (totalWeight > capacity) {
        // solution not found
        value = -1;
    }
    out[id] = value;
}