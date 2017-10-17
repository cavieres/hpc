#!/bin/bash

echo N\;X\;Y\;T\;Time\;

nvcc wave.cu -Xcompiler -fopenmp -o wave

./wave -N 512 -X 10 -Y 10 -T 300 -f wave.raw -t 300
./wave -N 512 -X 10 -Y 10 -T 1000 -f wave.raw -t 1000
./wave -N 512 -X 10 -Y 10 -T 10000 -f wave.raw -t 10000
./wave -N 1024 -X 10 -Y 10 -T 300 -f wave.raw -t 300
./wave -N 1024 -X 10 -Y 10 -T 1000 -f wave.raw -t 1000
./wave -N 1024 -X 10 -Y 10 -T 10000 -f wave.raw -t 10000
./wave -N 2048 -X 10 -Y 10 -T 300 -f wave.raw -t 300
./wave -N 2048 -X 10 -Y 10 -T 1000 -f wave.raw -t 1000
./wave -N 2048 -X 10 -Y 10 -T 10000 -f wave.raw -t 10000
./wave -N 4096 -X 10 -Y 10 -T 300 -f wave.raw -t 300
./wave -N 4096 -X 10 -Y 10 -T 1000 -f wave.raw -t 1000
./wave -N 4096 -X 10 -Y 10 -T 10000 -f wave.raw -t 10000

./wave -N 2048 -X 16 -Y 16 -T 300 -f wave.raw -t 300
./wave -N 2048 -X 32 -Y 16 -T 1000 -f wave.raw -t 1000
./wave -N 2048 -X 32 -Y 32 -T 10000 -f wave.raw -t 10000

