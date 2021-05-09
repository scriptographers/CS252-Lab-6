#!/bin/bash

if [ $# -ne 4 ]; then
    echo "Usage: bash run.sh <congestion_protocol> <flag_gen_file> <file_size> <flag_compile>"
    exit 1
fi

TCP_CONG_PROT=$1 # "reno" or "cubic"
FLAG_GEN=$2      # 0 or 1
FILE_SIZE=$3     # 100KB, 5MB etc
FLAG_COM=$4      # 0 or 1

if [ $FLAG_GEN -eq 1 ]; then

    # Generate send.txt
    base64 /dev/urandom | head -c $FILE_SIZE >send.txt

    if [ $? -eq 0 ]; then
        echo "File generation successful"
    else
        echo "File generation error"
        exit $?
    fi

fi

if [ $FLAG_COM -eq 1 ]; then

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

    echo ""

fi

# Run
./s.out &
P1=$!
sleep 2
./c.out $TCP_CONG_PROT &
P2=$!

# Print process IDs, in case of a bug, kill the processes directly
echo ""
echo "Server Process ID: $P1 | Client Process ID: $P2"
echo ""

FAIL1=0
FAIL2=0
wait $P1 || let "FAIL1=1"
wait $P2 || let "FAIL2=1"

echo ""

if [ "$FAIL1" == "0" ] && [ "$FAIL2" == "0" ]; then
    echo "Processes finished successfully"
else
    echo "Process(es) failed: $FAIL1 $FAIL2"
fi

echo ""

STATUS="$(
    cmp --silent send.txt recv.txt
    echo $?
)" # "$?" gives exit status for each comparison

# if status isn't equal to 0, then different files
if [[ $STATUS -ne 0 ]]; then
    echo "Different files"
else
    echo "Same files"
fi

if [ $FLAG_COM -eq 1 ]; then

    # Clean up
    rm s.out c.out
    echo "Clean up complete"

fi
