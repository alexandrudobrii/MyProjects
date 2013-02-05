#!/bin/bash

T=$1
FISIN=teste_t1/in$2_1.txt
FISOUT=output

export OMP_SCHEDULE="dynamic"
export OMP_NUM_THREADS=4

time ./paralel $T $FISIN $FISOUT