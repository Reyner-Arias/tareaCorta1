#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct matrix {
    int thread;
};

int **matriz1, **matriz2, **matrizRes;
int n = 0;

void rellenarMatriz(int** matriz){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matriz[i][j] = rand() % 10 - 4;
        }
    }
}

void rellenarCeros(int** matriz){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matriz[i][j] = 0;
        }
    }
}

void *rowXColumn(void *datosP)
{
    struct matrix *datos = datosP;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrizRes[datos->thread][i] += matriz1[datos->thread][j]*matriz2[j][i];
        }
    }
}

int main(){
    time_t t1;
    srand ( (unsigned) time (&t1));
    double promedio;

    printf("Digite el número que definirá la cantidad de filas y columnas de las matrices cuadradas aleatorias\n");
    scanf("%d", &n);
    double tiempos[n];

    for(int rep = 0; rep < 100; rep++){
        
        matriz1 = malloc(n * sizeof *matriz1);
        matriz2 = malloc(n * sizeof *matriz2);
        matrizRes = malloc(n * sizeof *matrizRes);

        for(int i = 0; i < n; i++){
            matriz1[i] = malloc(n * sizeof *matriz1[i]);
            matriz2[i] = malloc(n * sizeof *matriz2[i]);
            matrizRes[i] = malloc(n * sizeof *matrizRes[i]);
        }

        double executionTime = 0.0;

        rellenarMatriz(matriz1);
        rellenarMatriz(matriz2);
        rellenarCeros(matrizRes);

        clock_t begin = clock();

        pthread_t threads[n];
        struct matrix datosHilos[n];
        int thread = 0;

        for(int i = 0; i < n; i++){
            datosHilos[thread].thread = thread;
            pthread_create( &threads[thread], NULL, rowXColumn, &datosHilos[thread]);
            thread++;
        }

        for(int i = 0; i < n; i++){
            pthread_join( threads[i], NULL);
        }

        clock_t end = clock();
        executionTime += (double)(end - begin) / CLOCKS_PER_SEC;
        tiempos[thread] = executionTime;
        promedio += executionTime;
    }
    promedio = promedio/100;
    printf("El tiempo promedio fue de %f segundos\n", promedio);
    
    for(int i = 0; i < n; i++){
        free(matriz1[i]);
        free(matriz2[i]);
        free(matrizRes[i]);
    }
    free(matriz1);
    free(matriz2);
    free(matrizRes);

    return 0;
}