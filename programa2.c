#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define DIM 2
//#define T 8
#define K 2

typedef struct{
    float* data;
} Vector;

void mostrar_data(Vector* ELEMS, int N_DB, int dim);
Vector* inicializar_centroides(Vector* ELEMS, int N_DB, int dim);


int main(){
    int N_DB,i,j;
    Vector* ELEMS;

    //se ingresa la cantidad de vectores
    //printf("ingresa la cantidad de vectores: ");
    scanf("%d", &N_DB);
    // creacion de matriz
    ELEMS = (Vector *)malloc(N_DB * sizeof(Vector));

    for (i = 0; i < N_DB; i++) {
        Vector v;
        v.data = (float*)malloc(sizeof(float) * DIM);
        ELEMS[i] = v;
    }

    //rellenar matriz
    for (i = 0; i < N_DB; i++) {
        float* temp_data = ELEMS[i].data;
        for (j = 0; j < DIM; j++) {
            scanf("%f",&temp_data[j]);
        } 
    }
    
    Vector* centroides = inicializar_centroides(ELEMS, N_DB, DIM);
    
    mostrar_data(ELEMS, N_DB, DIM);
    printf("centroides\n");
    for (int i = 0; i<K; i++){
        for (int j= 0; j < DIM; j++) {
            float* temp_data = centroides[i].data;
            printf("%.2f ", temp_data[j]); 
        }
        printf("\n");
    }
    return 0;
}



Vector* inicializar_centroides(Vector* ELEMS, int N_DB, int dim){
    Vector* centroides = (Vector*)malloc(K*sizeof(Vector));
    Vector centroide_elegido;
    int centroide_idx, l=0;
    srand(time(NULL));

    for (int i = 0; i < K; i++) {
        centroide_idx = rand() % N_DB;
        printf("%d\n", centroide_idx);
        centroide_elegido = ELEMS[centroide_idx];
        centroides[i] = centroide_elegido;
    }
    return centroides;
}



void mostrar_data(Vector* ELEMS, int N_DB, int dim){
    for (int i = 0; i < N_DB; i++) {
        float* temp_data = ELEMS[i].data;
        for (int j = 0; j < dim; j++) {
            printf("%.2f",temp_data[j]);
            if(j!=dim-1){
                printf("-");
            }else{
                printf("\n");
            }
        }
        printf("\n");
    }

}



