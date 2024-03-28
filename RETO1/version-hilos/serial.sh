#!/bin/bash
#SBATCH --partition=all  	#Seleccione los nodos para el trabajo de todos el conjunto de nodos de cómputo del cluster
#SBATCH -o Serial.%j.out    	#Nombre del archivo de salida
#SBATCH -J Serial	        #Nombre del trabajo





gcc -DUSE_CLOCK -O3 jacobi1d.c timing.c -o jacobi1d


output_file="timing_threads.csv"

> $output_file

# Loop to run the program multiple times 
for N in 100000 200000 300000 400000 500000
do
    for NSTEPS in 1000 2000 3000 4000 5000
    do
        for THREADS in 2 4 8 12 16 32
        do
            echo "N = $N, NSTEPS = $NSTEPS, THREADS = $THREADS"
            for run in {1..10}
            do
                echo "   Run $run"
                ./jacobi1d $N $NSTEPS $THREADS u_serial.out >> $output_file
            done
        done
    done
done

echo "All executions completed. Results saved in $output_file"