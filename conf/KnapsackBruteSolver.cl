
typedef long int64_t;

__kernel void solve(__global char* instances,
                    __global int* objectsValues,
                    __global int* objectsWeights,
                     __global int* out, int n, int capacity)
{

    long id = get_global_id(0);
    long lid = get_local_id(0);


/*
    sprawdzic wplyw pamieci lokalnej.
    __local int oValues[32];
    __local int oWeights[32];

    event_t es[2];
    es[0] = async_work_group_copy(oValues, objectsValues, n, 0);
    es[1] = async_work_group_copy(oWeights, objectsWeights, n, 0);

    wait_group_events(2, es);
*/

    long value = 0;
    long totalWeight = 0;


    long num = get_global_id(0);
    long j = 0;
    do {
        bool take = (num & 1) == 1;
        if (take) {
            totalWeight += objectsWeights[j];
            value += objectsValues[j];
        }
        j++;
    } while(num >>= 1);


    if (totalWeight > capacity) {
        // solution not found
        value = -1;
    }
    long offset = get_global_offset(0);
    out[id-offset] = (int)value;

}