#!/bin/sh

tstamp=$(date +%s)
outfile=results_${tstamp}

touch $outfile

while [ $(wc -l $outfile | cut -d' ' -f 1) -lt 1000 ]
do
    mpirun -np 1 -host localhost ./program 10000 20000000 1000 10 >> $outfile
done


