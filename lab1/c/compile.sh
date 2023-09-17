#!/bin/bash
echo compile with -O0
gcc main.c -O0 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -O1
gcc main.c -O1 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -O2
gcc main.c -O2 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -O3
gcc main.c -O3 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -Os
gcc main.c -Os -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -Ofast
gcc main.c -Ofast -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -Og
gcc main.c -Og -o lab1 -lm
bash ./test.sh
