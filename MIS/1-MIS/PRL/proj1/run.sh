#!/bin/bash

if [ $# -lt 1 ];then
  numbers=10;
else
  numbers=$1;
fi;

mpic++ --prefix /usr/local/share/OpenMPI -o oes odd_even_sort.cpp
dd if=/dev/random bs=1 count=$numbers of=numbers
mpirun --prefix /usr/local/share/OpenMPI -np $numbers oes
rm -f oes numbers
