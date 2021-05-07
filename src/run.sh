#!/bin/bash

dd if=/dev/urandom bs=102400 count=1 status=none | base64 >send.txt

# Compile:

gcc -Wall -Wextra -o c.out client.c

if [ $? -eq 0 ]; then
    echo "Client compilation successful"
else
    echo "Client compilation error"
    exit $?
fi

gcc -Wall -Wextra -o s.out server.c

if [ $? -eq 0 ]; then
    echo "Server compilation successful"
else
    echo "Server compilation error"
    exit $?
fi

echo ""

# Run
./s.out &
./c.out &

wait

echo ""

STATUS="$(
    cmp --silent send.txt recv.txt
    echo $?
)" # "$?" gives exit status for each comparison

if [[ $STATUS -ne 0 ]]; then # if status isn't equal to 0, then execute code
    echo "Different files"
else
    echo "Same files"
fi

# Clean up
rm s.out
rm c.out
