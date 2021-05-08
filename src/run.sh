#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: bash run.sh <file_size>"
    exit 1
fi

FILE_SIZE=$1 # 100KB, 5MB etc

# Generate the send file

dd if=/dev/urandom bs=$FILE_SIZE count=1 status=none | base64 >send.txt
if [ $? -eq 0 ]; then
    echo "File generation successful"
else
    echo "File generation error"
    exit $?
fi
    
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
P1=$!
sleep 2
./c.out &
P2=$!

FAIL1=0
FAIL2=0
wait $P1 || let "FAIL1=1"
wait $P2 || let "FAIL2=1"

echo ""

if [ "$FAIL1" == "0" ] && [ "$FAIL2" == "0" ];
then
    echo "Processes finished successfully"
else
    echo "Process(es) failed: $FAIL1 $FAIL2"
fi

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
