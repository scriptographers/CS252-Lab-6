#!/bin/bash

# Compile:
gcc -o s.out client.c
gcc -o r.out server.c

./r.out &
./s.out &