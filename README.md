# Porr - Knapsack Problem Solver


# install dependencies

0 REQUIRED
- install cmake (version > 3.4)
- install opencl (drivers + api + libraries)
- install mpi


# build

```
mkdir build
cd build
cmake ../
make
```

binary files will be in directory output/bin

# run

-enter dir: cd `output/bin`

-run brute force:
```

        ./bruteFroce <<mode>> <<pathToConfigurationFile>>

```
examples:
```
        ./bruteForce seq data/pet2.dat
        ./bruteForce opencl data/pet2.dat
        mpirun -np 3 ./bruteForce data/pet2.dat

```




