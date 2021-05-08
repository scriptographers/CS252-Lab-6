#!/bin/bash

for j in 10ms 50ms 100ms
do
    for k in 0.1% 0.5% 1%
    do
    sudo tc qdisc change dev lo root netem loss $k delay $j
    echo "$j $k" >> data.txt
        for i in {1..20}
        do
            echo "$j $k $i"
            bash run.sh reno 0 5MB | grep "transfer:" >> data.txt
        done
    done
done


for j in 10ms 50ms 100ms
do
    for k in 0.1% 0.5% 1%
    do
    sudo tc qdisc change dev lo root netem loss $k delay $j
    echo "$j $k" >> data2.txt
        for i in {1..20}
        do
            echo "$j $k $i"
            bash run.sh cubic 0 5MB | grep "transfer:" >> data2.txt
        done
    done
done