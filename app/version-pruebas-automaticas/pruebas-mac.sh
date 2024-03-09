#!/bin/bash

# Compile the C program
gcc algoritmo.c -o test

# Specify the output file
output_file="resultado_secuencial.csv"

> $output_file

# Loop to run the program multiple times for each matrix size

for matrix_size in 500 1000 1500 2000 2500 3000
do
    echo "Matrix size: $matrix_size."

    # Run the program 10 times
    for run in {1..10}
    do
        echo "   Run $run"
        ./test $matrix_size >> $output_file
    done
done


echo "All executions completed. Results saved in $output_file"