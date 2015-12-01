#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 test_logfile"
    exit 1
fi

awk '{ sum1 += $7; sum2 += $8; cnt1++; cnt2++;} END { printf ( "%6.6f\n%6.6f\n", sum1/cnt1, sum2/cnt2);}' $1

