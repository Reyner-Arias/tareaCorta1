#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct matrix {
    int* row;
    int* column;
    int* totales;
    int thread;
};

int n = 0;

void rellenarMatriz(int matriz[][n]){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matriz[i][j] = rand() % 10 - 4;
        }
    }
}

void *rowXColumn(void *datosP)
{
    struct matrix *datos = datosP;
    datos->totales[datos->thread] = 0;
    for(int i = 0; i < n; i++){
        datos->totales[datos->thread] += datos->row[i]*datos->column[i];
    }
}

int main(){
    time_t t1;
    srand ( (unsigned) time (&t1)); 
    double promedio;

    printf("Digite el número que definirá la cantidad de filas y columnas de las matrices cuadradas aleatorias\n");
    scanf("%d", &n);

    if(n>0&&n<=180){
        for(int rep = 0; rep < 100; rep++){
            int matriz1[n][n];
            int matriz2[n][n];
            int matriz2T[n][n];
            int matrizRes[n][n];

            double executionTime = 0.0;

            rellenarMatriz(matriz1);
            rellenarMatriz(matriz2);

            for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                    matriz2T[i][j] = matriz2[j][i];
                }
            }

            clock_t begin = clock();

            pthread_t threads[n*n];
            struct matrix datosHilos[n*n];
            int totales[n*n];
            int thread = 0;

            for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                    
                    datosHilos[thread].row = matriz1[i];
                    datosHilos[thread].column = matriz2T[j];
                    datosHilos[thread].totales = totales;
                    datosHilos[thread].thread = thread;
                    
                    pthread_create( &threads[thread], NULL, rowXColumn, &datosHilos[thread]);
                    thread++;
                }
            }

            for(int i = 0; i < n*n; i++){
                pthread_join( threads[i], NULL);
            }
            for(int i = 0; i < n*n; i++){
                matrizRes[(i-(i%n))/n][i%n] = totales[i];
            }

            clock_t end = clock();
            executionTime += (double)(end - begin) / CLOCKS_PER_SEC;
            promedio += executionTime;
        }
        promedio = promedio/100;
        printf("El tiempo promedio fue de %f segundos\n", promedio);
    } else {
        printf("Matrices con un n > 180 o n < 1, no están permitidas\n");
    }

    return 0;
}