#!/bin/bash

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