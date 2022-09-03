#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void rowXColumn(int row)
{
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrizRes[row][i] += matriz1[row][j]*matriz2[j][i];
        }
    }
}

int main(){
    time_t t1;
    srand ( (unsigned) time (&t1));
    double promedio = 0.0;

    printf("Digite el número que definirá la cantidad de filas y columnas de las matrices cuadradas aleatorias\n");
    scanf("%d", &n);
    double tiempos[n];
    pid_t parent = getpid();

    matriz1 = malloc(n * sizeof *matriz1);
    matriz2 = malloc(n * sizeof *matriz2);
    matrizRes = malloc(n * sizeof *matrizRes);

    for(int i = 0; i < n; i++){
        matriz1[i] = malloc(n * sizeof *matriz1[i]);
        matriz2[i] = malloc(n * sizeof *matriz2[i]);
        matrizRes[i] = malloc(n * sizeof *matrizRes[i]);
    }

    for(int rep = 0; rep < 40; rep++){
        double executionTime = 0.0;

        rellenarMatriz(matriz1);
        rellenarMatriz(matriz2);
        rellenarCeros(matrizRes);

        clock_t begin = clock();

        pid_t processes[n];
        int counter = 0;
        int state[n]; 
        
        int fds[n][2];
        for(int i = 0; i < n; i++){
            if (pipe(fds[i]) == -1){ 
                printf("Ocurrió un error con el pipeline\n");
                exit(1);
            }
        }

        do {
            processes[counter] = fork();
            if (processes[counter]  < 0) {
                printf("Ocurrió un error con el fork\n");
                exit(1);
            }
            
            if (getpid() != parent){
                close(fds[counter][0]);
                rowXColumn(counter);
                write(fds[counter][1], matrizRes[counter], (sizeof(matrizRes[counter])*n)+1);
                close(fds[counter][1]);
                for(int i = 0; i < n; i++){
                    if(i!=counter){
                        close(fds[i][0]);
                        close(fds[i][1]);
                    }
                }
                rep = 100;
            } 
            counter++;
        } while ((getpid() == parent && processes[counter] != -1) && (counter < n));

        if(parent == getpid()){
            for (int i = 0; i < n; i++){
                waitpid(processes[i], &state[i], 0);
            }
            for (int i = 0; i < n; i++){
                close(fds[i][1]);
                read(fds[i][0], matrizRes[i], sizeof(matrizRes[counter])*n);
                close(fds[i][0]);
            }
            clock_t end = clock();
            tiempos[rep] = executionTime;
            executionTime += (double)(end - begin) / CLOCKS_PER_SEC;
        } 
    }
    if(parent == getpid()){
        promedio = promedio/100;
        printf("El tiempo promedio fue de %f segundos\n", promedio);
    } 
    
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