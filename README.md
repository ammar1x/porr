# porr


# install dependencies

- install cmake (version > 3.4)
- install opencl (drivers + api + libraries)
- install googletest
- install mpi


# build

- mkdir build
- cmake ../

bin files will be in directory output/bin

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

```




