#!/bin/bash

tstamp=$(date +%s)
logfile=test_${tstamp}.log

if [ $# -ne 4 ]
then
    echo "Usage: $0 num_tests vn en mw"
    exit 1
fi

for i in $(seq 1 $1)
do
    echo -ne "$i\t" >> $logfile
    ./program $2 $3 $4 10 >> $logfile
done
