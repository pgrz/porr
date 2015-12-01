#!/bin/sh

tstamp=$(date +%s)
logfile=test_${tstamp}.log

if [ $# -ne 4 ]
then
    echo "Usage: $0 num_tests vn en mw"
    exit 1
fi

for i in $(seq 1 $1)
do
    ./program $2 $3 $4 10 >> $logfile
done


ls $logfile

awk '{ sum1 += $6; sum2 += $7; cnt1++; cnt2++;} END { print sum1/cnt1; print sum2/cnt2;}' $logfile
