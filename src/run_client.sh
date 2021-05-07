#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: bash run_client.sh <file_size>"
    exit 1
fi

FILE_SIZE=$1 # 100KB, 5MB etc

dd if=/dev/urandom bs=$FILE_SIZE count=1 status=none | base64 >send.txt
    
# Compile:

gcc -Wall -Wextra -o c.out client.c

if [ $? -eq 0 ]; then
    echo "Client compilation successful"
else
    echo "Client compilation error"
    exit $?
fi

./c.out