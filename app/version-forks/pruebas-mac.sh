#!/bin/sh
for j in {1..10}
do
    for i in 10 100 200 400 600 800 1000 2000
    do
        for k in {2..4}
        do
            ./a.out $i $k>> resultados-mac.txt
        done      
    done
done