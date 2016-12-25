

__kernel void solve(__global char* instances,
                    __global int* objectsValues,
                    __global int* objectsWeights,
                     __global int* out, int n, int capacity)
{

    int id = get_global_id(0);
    int lid = get_local_id(0);


/*
    sprawdzic wplyw pamieci lokalnej.
    __local int oValues[32];
    __local int oWeights[32];

    event_t es[2];
    es[0] = async_work_group_copy(oValues, objectsValues, n, 0);
    es[1] = async_work_group_copy(oWeights, objectsWeights, n, 0);

    wait_group_events(2, es);
*/

    int value = 0;
    int totalWeight = 0;


    int num = get_global_id(0);
    int j = 0;
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
    int offset = get_global_offset(0);
    out[id-offset] = value;

}