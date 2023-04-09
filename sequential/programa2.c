#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DIM 2
//#define T 8
#define K 4

typedef struct{
    float* data;
} Vector; // estructura para guardar los puntos y hacer mas simple la vista

void mostrar_data(Vector* ELEMS, int N_DB, int dim);
Vector* inicializar_centroides(Vector* ELEMS, int N_DB, int dim);
Vector* inicializar_centroides_kmeanspp(Vector* ELEMS, int N_DB, int dim);

float calcular_distancia(float* punto_1, float* centroide, int dim);
double distancia_cuadrado(Vector x, Vector y, int dim);

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
    // centroides = inicializar_centroides_kmeanspp(ELEMS, N_DB, DIM);
    
    mostrar_data(ELEMS, N_DB, DIM);
    printf("\ncentroides\n\n");
    mostrar_data(centroides,K,DIM);
    printf("%.2f\n", calcular_distancia(&ELEMS->data[0], &centroides->data[0] , DIM));
   
    // meter en un bucle
    int* idx_centroides_por_punto = (int*)calloc(N_DB,sizeof(int));// esto puede ser una funcion para mostrar lo que tengo que mostrar en pantalla

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
    printf("\n\nRecalculo de centroides:\n");

    recalcular_centroides(centroides, idx_centroides_por_punto, ELEMS, K, N_DB, DIM);
    mostrar_data(centroides,K,DIM);
    return 0;
}


Vector* inicializar_centroides_kmeanspp(Vector* ELEMS, int N_DB, int dim) {
    Vector* centroides = (Vector*)malloc(K*sizeof(Vector));
    srand(time(NULL));

    // Seleccionar el primer centroide al azar
    int centroide_idx = rand() % N_DB;
    centroides[0] = ELEMS[centroide_idx];

    // Seleccionar los siguientes centroides usando k-means++
    for (int i = 1; i < K; i++) {
        // Calcular las distancias al cuadrado de cada punto al centroide más cercano
        double* distancias = (double*)malloc(N_DB*sizeof(double));
        for (int j = 0; j < N_DB; j++) {
            double min_dist = INFINITY;
            for (int l = 0; l < i; l++) {
                double dist = distancia_cuadrado(ELEMS[j], centroides[l], dim);
                if (dist < min_dist) {
                    min_dist = dist;
                }
            }
            distancias[j] = min_dist;
        }

        // Seleccionar el siguiente centroide usando las distancias al cuadrado
        double dist_total = 0.0;
        for (int j = 0; j < N_DB; j++) {
            dist_total += distancias[j];
        }
        double r = ((double)rand() / RAND_MAX) * dist_total;
        double acumulador = 0.0;
        for (int j = 0; j < N_DB; j++) {
            acumulador += distancias[j];
            if (acumulador >= r) {
                centroides[i] = ELEMS[j];
                break;
            }
        }

        free(distancias);
    }

    return centroides;
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

double distancia_cuadrado(Vector x, Vector y, int dim) {
    double dist = 0.0;
    for (int i = 0; i < dim; i++) {
        dist += pow(x.data[i] - y.data[i], 2);
    }
    return dist;
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



void recalcular_centroides(Vector* centroides, int* idx_centroides_por_punto, Vector* ELEMS, int num_centroides, int num_puntos, int dim){
    // encontrar los puntos que pertenecen a cada centroide y calcular la media de sus coordenadas(esta media sera el nuevo centroide)
    for(int i = 0; i < num_centroides; i++){
        float *sum_puntos = (float*)calloc(dim,sizeof(float));
        // memset(centroides[i].data, 0, dim*sizeof(float));

        int count = 0; // cantidad de puntos que pertenecen al centroide i
        for (int j = 0; j < num_puntos; j ++) {
            if (idx_centroides_por_punto[j] == i) {
                float* punto_temp = ELEMS[j].data;
                for (int l = 0; l < dim; l++) {
                    sum_puntos[l] += punto_temp[l]; // sumo cada coordenada de todos los puntos
                    // centroides[i].data[l] += punto_temp[l]; // sumo cada coordenada de todos los puntos
                }
                count++;
            
            }
        
        }
        if(count != 0){
            for (int j = 0; j < dim; j++) {
                centroides[i].data[j] = sum_puntos[j] / count; 
                // centroides[i].data[j] /= count; 
            } 
            
        }
        free(sum_puntos);
    }
}

