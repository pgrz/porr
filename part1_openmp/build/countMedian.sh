#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 test_logfile"
    exit 1
fi

mid=$(echo "$(wc -l < $1) / 2" | bc )

sort -k 7 $1 | head -n $mid | tail -n1 | awk '{print $7}'
sort -k 8 $1 | head -n $mid | tail -n1 | awk '{print $8}'
