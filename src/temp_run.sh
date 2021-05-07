#!/bin/bash

# Compile:
gcc -Wall -Wextra -o s.out client.c 

if [ $? -eq 0 ]; then
    echo "Client compilation successful"
else
    echo "Client compilation error"
    exit $?
fi

gcc -Wall -Wextra -o r.out server.c

if [ $? -eq 0 ]; then
    echo "Server compilation successful"
else
    echo "Server compilation error"
    exit $?
fi

# Run
./r.out &
./s.out &