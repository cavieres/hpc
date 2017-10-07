# HPC
High Performance Computing, Lab1: SIMD-SSE


## Lab-1

### Compile and execution:

```
gcc simdsort.c stack-pointer.c -o simdsort -msse4 && ./simdsort -i n268435456.raw -o ordenada.raw -N 32 -d 1
```

Or

```
make && ./simdsort -i n268435456.raw -o ordenada.raw -N 32 -d 1
```

## Lab-2

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

## Lab-3

### Compile and execution:

```
nvcc wave.cu -Xcompiler -fopenmp
```

