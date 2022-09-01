#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n = 0;

void rellenarMatriz(int matriz[][n]){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matriz[i][j] = rand() % 20 + 1;
        }
    }
}

int main(){
    time_t t1;
    struct tm *currentTime, *finishTime;
    srand ( (unsigned) time (&t1)); 

    printf("Digite el número que definirá la cantidad de filas y columnas de las matrices cuadradas aletorias\n");
    scanf("%d", &n);

    int matriz1[n][n];
    int matriz2[n][n];

    rellenarMatriz(matriz1);
    rellenarMatriz(matriz2);

    time(&t1);
    currentTime = localtime( &t1 );
    printf("Current local time and date: %s", asctime(currentTime));

    for(int i = 0; i < 1000000000; i++)

    time(&t1);
    finishTime = localtime( &t1 );
    printf("Current local time and date: %s", asctime(finishTime));
    
    return 0;
}