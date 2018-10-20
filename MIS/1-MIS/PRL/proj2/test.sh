#!/usr/bin/env bash

if [ $# -eq 1 ]; then
  n_count=$1;
else
  >&2 echo "Invalid number of parameters, expected 1 got" $#
  exit -1
fi;

mpic++ --std=c++11 --prefix /usr/local/share/OpenMPI -o es es.cpp
dd if=/dev/random bs=1 count=$n_count of=numbers &> /dev/null
((n_count++))
mpirun --prefix /usr/local/share/OpenMPI -np $n_count es
rm -f es numbers
