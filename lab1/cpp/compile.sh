#!/bin/bash
echo compile with -O0
g++ main.cpp -O0 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -O1
g++ main.cpp -O1 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -O2
g++ main.cpp -O2 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -O3
g++ main.cpp -O3 -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -Os
g++ main.cpp -Os -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -Ofast
g++ main.cpp -Ofast -o lab1 -lm
bash ./test.sh
echo "--------------------"
echo compile with -Og
g++ main.cpp -Og -o lab1 -lm
bash ./test.sh
