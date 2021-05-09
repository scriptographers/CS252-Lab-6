#!/bin/bash

sudo tc qdisc add dev lo root netem loss 1% delay 100ms &>/dev/null
# Needed to "add" once before "change"

# Generate send.txt
base64 /dev/urandom | head -c 5MB >send.txt

if [ $? -eq 0 ]; then
    echo "File generation successful"
else
    echo "File generation error"
    exit $?
fi

# Compile client:
gcc -Wall -Wextra -o c.out client.c

if [ $? -eq 0 ]; then
    echo "Client compilation successful"
else
    echo "Client compilation error"
    exit $?
fi

# Compile server:
gcc -Wall -Wextra -o s.out server.c

if [ $? -eq 0 ]; then
    echo "Server compilation successful"
else
    echo "Server compilation error"
    exit $?
fi

# TCP-Reno Experiments
rm -rf reno.txt
echo ""
echo "Starting experiments on TCP-Reno:"
for j in 10ms 50ms 100ms; do
    for k in 0.1% 0.5% 1%; do
        sudo tc qdisc change dev lo root netem loss $k delay $j
        echo "$j $k" >>reno.txt
        for i in {1..20}; do
            echo "TCP-Reno Delay:$j Loss:$k Run#:$i"
            bash run.sh reno 0 5MB 0 | grep "transfer:" >>reno.txt
        done
    done
done

# TCP-Cubic Experiments
rm -rf cubic.txt
echo ""
echo "Starting experiments on TCP-Cubic:"
for j in 10ms 50ms 100ms; do
    for k in 0.1% 0.5% 1%; do
        sudo tc qdisc change dev lo root netem loss $k delay $j
        echo "$j $k" >>cubic.txt
        for i in {1..20}; do
            echo "TCP-Cubic Delay:$j Loss:$k Run#:$i"
            bash run.sh cubic 0 5MB 0 | grep "transfer:" >>cubic.txt
        done
    done
done

# Clean up
rm s.out c.out
echo ""
echo "Experiments completed"