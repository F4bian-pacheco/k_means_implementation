#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/*
En este código, se implementó el algoritmo de clustering k-means. 
El programa mostraba resultados inesperados en la función de recalculo de centroides, 
donde se encontraron cambios no intencionales en el array ELEMS, que debería ser de solo lectura.

Después de revisar el código y hacer algunas pruebas, se descubrió que el problema estaba en la función de inicialización de centroides. 
Al asignar el valor del vector aleatorio a la variable centroide_elegido, se estaba asignando su dirección de memoria en lugar de copiar el valor del vector. 
Entonces, al cambiar el valor de centroide_elegido, también se cambiaba el valor del vector correspondiente en ELEMS.

La solución fue copiar el valor del vector en lugar de asignar la dirección de memoria, usando la función memcpy. 
Después de hacer este cambio, el programa funcionó correctamente y se evitó la corrupción no intencional de los datos en ELEMS.
*/

//TODO
//* medir el tiempo

#define DIM 20
//#define T 8
#define K 3

typedef struct{
    float* data;
} Vector; // estructura para guardar los puntos y hacer mas simple la vista

void mostrar_data(Vector* ELEMS, int N_DB, int dim);
Vector* inicializar_centroides(Vector *ELEMS, int N_DB, int dim);
Vector* inicializar_centroides_kmeanspp(Vector* ELEMS, int N_DB, int dim);// mas aleatorio

float calcular_distancia(float* punto_1, float* centroide, int dim);
double distancia_cuadrado(Vector x, Vector y, int dim); // esta se usa en el k-meanspp

int centroide_mas_cercano(float* punto, int dim_pto, Vector* centroides, int num_centroides);
void recalcular_centroides(Vector* centroides, int* idx_centroides_por_punto, Vector* ELEMS, int num_centroides, int num_puntos, int dim);

int comparar_centroides(Vector* centroides, Vector* centroides_anteriores, int num_centroides, int dim);

void mostrar_indices(int* idx_centroides_por_punto, int N_DB);

int main(){
    int N_DB,i,j,k = 0;
    Vector *ELEMS, *centroides;
    struct timeval t1, t2;
    float real_time;

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
    
    gettimeofday(&t1, 0);
    centroides = inicializar_centroides(ELEMS, N_DB, DIM);
    // centroides = inicializar_centroides_kmeanspp(ELEMS, N_DB, DIM);
    
    // mostrar_data(ELEMS, N_DB, DIM);
    // printf("\nCentroides iniciales:\n\n");
    // mostrar_data(centroides,K,DIM);
    // printf("%.2f\n", calcular_distancia(&ELEMS->data[0], &centroides->data[0] , DIM));
    
    // meter en un bucle
    Vector* centroides_anteriores = (Vector*)malloc(K*sizeof(Vector));
    for (int i = 0; i < K; i++) {
        Vector v;
        v.data = (float*)malloc(sizeof(float) * DIM);
        memcpy(v.data, centroides[i].data, DIM*sizeof(float));
        centroides_anteriores[i] = v;
    }

    // printf("\nCentroides anteriores:\n\n");
    // mostrar_data(centroides_anteriores,K,DIM);
    
    while (1)
    {
        int* idx_centroides_por_punto = (int*)calloc(N_DB,sizeof(int));// esto puede ser una funcion para mostrar lo que tengo que mostrar en pantalla

        for (int i = 0; i < N_DB; i++) {
            float* punto = ELEMS[i].data;
            int indice_centroide_cercano = centroide_mas_cercano(punto, DIM, centroides, K);
            idx_centroides_por_punto[i] = indice_centroide_cercano; 
        }

        recalcular_centroides(centroides, idx_centroides_por_punto, ELEMS, K, N_DB, DIM);
        // mostrar_data(centroides,K,DIM);
        // printf("\n\n");
        // mostrar_data(centroides_anteriores,K,DIM);

        if (comparar_centroides(centroides, centroides_anteriores, K, DIM) == 1){
            // printf("\nCentroides finales:\n\n");
            // mostrar_data(centroides,K,DIM);
            // printf("\n\n");
            // mostrar_data(centroides_anteriores,K,DIM);
            mostrar_indices(idx_centroides_por_punto, N_DB);
            break;
        }
        
        for (int i = 0; i < K; i++) {
            memcpy(centroides_anteriores[i].data, centroides[i].data, DIM*sizeof(float));
        }
        
        free(idx_centroides_por_punto);
        // mostrar_data(centroides,K,DIM);

        k++;
    }
    // printf("Iteraciones: %d\n",k);
    // printf("\nCentroides finales:\n\n");
    // mostrar_data(centroides,K,DIM);
    // printf("\n\n");
    // mostrar_data(ELEMS, N_DB, DIM);
    gettimeofday(&t2, 0);
    real_time = (t2.tv_sec - t1.tv_sec) + (float)(t2.tv_usec - t1.tv_usec) / 1000000;
    printf("\nTiempo Real = %f\n", real_time);     

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



Vector* inicializar_centroides(Vector *ELEMS, int N_DB, int dim){
    Vector* centroides = (Vector*)malloc(K*sizeof(Vector));
    Vector centroide_elegido;
    int centroide_idx, l=0;
    srand(time(NULL));

    for (int i = 0; i < K; i++) {
        centroide_idx = rand() % N_DB;
        centroide_elegido.data = (float*)malloc(sizeof(float) * dim);
        memcpy(centroide_elegido.data, ELEMS[centroide_idx].data, dim*sizeof(float));
        // centroide_elegido = ELEMS[centroide_idx];
        centroides[i] = centroide_elegido;
    }
    return centroides;
}



void mostrar_data(Vector* vector, int N_DB, int dim){
    for (int i = 0; i < N_DB; i++) {
        float* temp_data = vector[i].data;
        printf("%d -> ",i);
        for (int j = 0; j < dim; j++) {
            printf("%.3f ",temp_data[j]);
        }
        printf("\n");
    }

}

void mostrar_indices(int* idx_centroides_por_punto, int N_DB){
    // printf("\nCentroides mas cercanos para cada punto:\n");
    for (int i = 0; i < N_DB; i++) {
        printf("%d\n",idx_centroides_por_punto[i]+1);
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

int comparar_centroides(Vector* centroides, Vector* centroides_anteriores, int num_centroides, int dim){
    int iguales = 1;
    for (int i = 0; i < num_centroides; i++) {
        for (int j = 0; j < dim; j++) {
            if (centroides[i].data[j] != centroides_anteriores[i].data[j]) {
                iguales = 0;
                break;
            }
        }
    }
    return iguales;
}

