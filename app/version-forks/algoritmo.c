#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Función para generar números aleatorios entre 1 y 5
int generarNumeroAleatorio()
{
    return rand() % 5 + 1;
}

// Función para imprimir una matriz
void imprimirMatriz(int **matriz, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Función para multiplicar una parte de dos matrices cuadradas
void multiplicarMatricesParcial(int **matrizA, int **matrizB, int **resultado, int n, int inicioFila, int finFila)
{
    for (int i = inicioFila; i < finFila; i++)
    {
        for (int j = 0; j < n; j++)
        {
            resultado[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                resultado[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // Verificar si se proporcionó el valor de n y el número de forks como argumentos
    if (argc != 3)
    {
        printf("Uso: %s <valor_de_n> <num_forks>\n", argv[0]);
        return 1;
    }

    // Obtener el valor de n y el número de forks desde los argumentos de línea de comandos
    int n = atoi(argv[1]);
    int numForks = atoi(argv[2]);

    // Inicializar la semilla para generar números aleatorios
    srand(time(NULL));

    // Crear matrices y reservar memoria dinámica
    int **matrizA, **matrizB, **resultado;
    matrizA = (int **)malloc(n * sizeof(int *));
    matrizB = (int **)malloc(n * sizeof(int *));
    resultado = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        matrizA[i] = (int *)malloc(n * sizeof(int));
        matrizB[i] = (int *)malloc(n * sizeof(int));
        resultado[i] = (int *)malloc(n * sizeof(int));
    }

    // Inicializar las matrices con valores aleatorios
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrizA[i][j] = generarNumeroAleatorio();
            matrizB[i][j] = generarNumeroAleatorio();
        }
    }

    // Imprimir las matrices originales
    printf("Matriz A:\n");
    imprimirMatriz(matrizA, n);

    printf("\nMatriz B:\n");
    imprimirMatriz(matrizB, n);

    // Medir el tiempo antes de ejecutar la función de multiplicación
    clock_t inicio = clock();

    // Dividir el trabajo entre los forks
    int filasPorFork = n / numForks;
    int inicioFila = 0;
    int finFila = filasPorFork;

    for (int i = 0; i < numForks; i++)
    {
        if (i == numForks - 1)
        {
            // Ajustar la última iteración para manejar filas adicionales si n no es divisible por numForks
            finFila = n;
        }

        pid_t pid = fork();

        if (pid == 0)
        {
            // Proceso hijo
            multiplicarMatricesParcial(matrizA, matrizB, resultado, n, inicioFila, finFila);
            exit(0);
        }
        else if (pid < 0)
        {
            // Error al crear el proceso hijo
            perror("Error en fork");
            return 1;
        }

        // Actualizar los índices de las filas para la próxima iteración
        inicioFila = finFila;
        finFila += filasPorFork;
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < numForks; i++)
    {
        wait(NULL);
    }

    // Medir el tiempo después de ejecutar la función de multiplicación
    clock_t fin = clock();

    // Calcular el tiempo transcurrido
    double tiempo_transcurrido = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
    tiempo_transcurrido = tiempo_transcurrido * 1000; // convertir a milisegundos

    // Imprimir la matriz resultante
    printf("\nMatriz Resultado:\n");
    imprimirMatriz(resultado, n);

    // Imprimir el tiempo de ejecución
    printf("\nTiempo de ejecucion: %.6f milisegundos\n", tiempo_transcurrido);

    // Liberar la memoria
    for (int i = 0; i < n; i++)
    {
        free(matrizA[i]);
        free(matrizB[i]);
        free(resultado[i]);
    }
    free(matrizA);
    free(matrizB);
    free(resultado);

    return 0;
}
