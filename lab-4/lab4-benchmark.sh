#!/bin/bash

#echo N\;X\;Y\;T\;Time\;

mpicc gol.c -fopenmp -std=c99 -o gol

mpirun -np 32 ./gol -r 128 -c 128 -i 150 -m 0 -t 10 -s 100
mpirun -np 64 ./gol -r 128 -c 128 -i 150 -m 0 -t 10 -s 100
mpirun -np 128 ./gol -r 128 -c 128 -i 150 -m 0 -t 10 -s 100
mpirun -np 256 ./gol -r 128 -c 128 -i 150 -m 0 -t 10 -s 100

mpirun -np 32 ./gol -r 256 -c 256 -i 150 -m 0 -t 10 -s 100
mpirun -np 64 ./gol -r 256 -c 256 -i 150 -m 0 -t 10 -s 100
mpirun -np 128 ./gol -r 256 -c 256 -i 150 -m 0 -t 10 -s 100
mpirun -np 256 ./gol -r 256 -c 256 -i 150 -m 0 -t 10 -s 100

mpirun -np 32 ./gol -r 512 -c 512 -i 150 -m 0 -t 10 -s 100
mpirun -np 64 ./gol -r 512 -c 512 -i 150 -m 0 -t 10 -s 100
mpirun -np 128 ./gol -r 512 -c 512 -i 150 -m 0 -t 10 -s 100
mpirun -np 256 ./gol -r 512 -c 512 -i 150 -m 0 -t 10 -s 100

mpirun -np 32 ./gol -r 128 -c 128 -i 300 -m 0 -t 10 -s 100
mpirun -np 64 ./gol -r 128 -c 128 -i 300 -m 0 -t 10 -s 100
mpirun -np 128 ./gol -r 128 -c 128 -i 300 -m 0 -t 10 -s 100
mpirun -np 256 ./gol -r 128 -c 128 -i 300 -m 0 -t 10 -s 100

mpirun -np 32 ./gol -r 256 -c 256 -i 300 -m 0 -t 10 -s 100
mpirun -np 64 ./gol -r 256 -c 256 -i 300 -m 0 -t 10 -s 100
mpirun -np 128 ./gol -r 256 -c 256 -i 300 -m 0 -t 10 -s 100
mpirun -np 256 ./gol -r 256 -c 256 -i 300 -m 0 -t 10 -s 100

mpirun -np 32 ./gol -r 512 -c 512 -i 300 -m 0 -t 10 -s 100
mpirun -np 64 ./gol -r 512 -c 512 -i 300 -m 0 -t 10 -s 100
mpirun -np 128 ./gol -r 512 -c 512 -i 300 -m 0 -t 10 -s 100
mpirun -np 256 ./gol -r 512 -c 512 -i 300 -m 0 -t 10 -s 100


