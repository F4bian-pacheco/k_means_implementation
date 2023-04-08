#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define DIM 2
//#define T 8
#define K 3

typedef struct{
    float* data;
} Vector; // estructura para guardar los puntos y hacer mas simple la vista

void mostrar_data(Vector* ELEMS, int N_DB, int dim);
Vector* inicializar_centroides(Vector* ELEMS, int N_DB, int dim);
float calcular_distancia(float* punto_1, float* centroide, int dim);
int centroide_mas_cercano(float* punto, int dim_pto, Vector* centroides, int num_centroides);
void recalcular_centroides(Vector* centroides, int* idx_centroides_por_punto, Vector* ELEMS, int num_centroides, int num_puntos, int dim);



int main(){
    int N_DB,i,j;
    Vector *ELEMS, *centroides;

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
    
    centroides = inicializar_centroides(ELEMS, N_DB, DIM);
    
    mostrar_data(ELEMS, N_DB, DIM);
    printf("\ncentroides\n\n");
    mostrar_data(centroides,K,DIM);
    printf("%.2f\n", calcular_distancia(&ELEMS->data[0], &centroides->data[0] , DIM));
    
    int* idx_centroides_por_punto = (int*)malloc(N_DB*sizeof(int));// esto puede ser una funcion para mostrar lo que tengo que mostrar en pantalla

    for (int i = 0; i < N_DB; i++) {
        float* punto = ELEMS[i].data;
        int indice_centroide_cercano = centroide_mas_cercano(punto, DIM, centroides, K);
        idx_centroides_por_punto[i] = indice_centroide_cercano; 
    }

    printf("Centroides mas cercanos para cada punto:\n");
    for (int i = 0; i < N_DB; i++) {
        printf("%d -> ",idx_centroides_por_punto[i]);
        for (int j = 0; j < DIM; j++) {
            float* temp_data = ELEMS[i].data;
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
        centroide_elegido = ELEMS[centroide_idx];
        centroides[i] = centroide_elegido;
    }
    return centroides;
}



void mostrar_data(Vector* ELEMS, int N_DB, int dim){
    for (int i = 0; i < N_DB; i++) {
        float* temp_data = ELEMS[i].data;
        printf("%d -> ",i);
        for (int j = 0; j < dim; j++) {
            printf("%.2f ",temp_data[j]);
        }
        printf("\n");
    }

}


float calcular_distancia(float* punto_1, float* centroide, int dim){
    float distancia = 0;
    float diff = 0;
    for (int i = 0; i < dim; i++) {
        diff += powf(punto_1[i]-centroide[i], 2.0);
    }
    distancia = sqrtf(diff);
    return distancia;
}


int centroide_mas_cercano(float* punto, int dim_pto, Vector* centroides, int num_centroides){
    int idx_centroide_mas_cercano = 0;
    float minima_distancia = INFINITY;
    for(int i = 0; i < num_centroides; i++){
        float distancia_temp = calcular_distancia(punto, centroides[i].data, dim_pto);
        if (distancia_temp < minima_distancia) {
            minima_distancia = distancia_temp;
            idx_centroide_mas_cercano = i;
        }
    }
    return idx_centroide_mas_cercano;
}
