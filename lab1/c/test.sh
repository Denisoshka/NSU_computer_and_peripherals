#!/bin/bash
n=4000000000
x=0.125
echo "n = $((n))"
sync
echo "---$((n))---"
sync
./lab1 $((n)) $(bc<<<"scale=3;$x")
echo "---$((n*2))---"
sync
./lab1 $((n*2)) $(bc<<<"scale=3;$x")
echo "---$((n*3))---"
sync
./lab1 $((n*3)) $(bc<<<"scale=3;$x")

