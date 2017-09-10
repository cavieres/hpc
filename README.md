# HPC
High Performance Computing, Lab1: SIMD-SSE


## Compile and execution:

```
gcc simdsort.c stack-pointer.c -o simdsort -msse4 && ./simdsort -i n268435456.raw -o ordenada.raw -N 32 -d 1
```

Or

```
make && ./simdsort -i n268435456.raw -o ordenada.raw -N 32 -d 1
```