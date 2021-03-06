# HPC
High Performance Computing, semester's laboratories.


## Lab-1 (SIMD-SSE)

### Compile and execution:

```
gcc simdsort.c stack-pointer.c -o simdsort -msse4 && ./simdsort -i n268435456.raw -o ordenada.raw -N 32 -d 1
```

Or

```
make && ./simdsort -i n268435456.raw -o ordenada.raw -N 32 -d 1
```

## Lab-2 (OpenMP)

### Compile and execution:

```
gcc wave.c -o wave -fopenmp && ./wave -N 10 -T 1 -H 1 -f wave.raw -t 1
```

### Bulk execution
```
gcc exec-its-bchmks.c -o exec-its-bchmks && ./exec-its-bchmks -> exec-its-bchmks.txt
chmod +x lab2-benchmark.sh
./lab2-benchmark.sh > lab2-benchmark.csv
```

## Lab-3 (CUDA)

### Compile and execution:

```
nvcc wave.cu -Xcompiler -fopenmp -o wave && ./wave -N 256 -X 10 -Y 10 -T 2 -f wave.raw -t 2
```

## Lab-4 (OpenMPI)

### Compile and execution:

```
mpicc gol.c -fopenmp -o gol && mpirun -np 4 ./gol -r 8 -c 8 -i 2 -m 0 -t 2 -s 3
```