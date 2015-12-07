#!/bin/sh

mpirun -np $(grep 'processor' /proc/cpuinfo | wc -l) -host localhost ./program $@
