#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>


#define DIM 20
#define T 8
#define K 3

typedef struct
{
    float *data;
} Vector; // estructura para guardar los puntos y hacer mas simple la vista

void mostrar_data(Vector *ELEMS, int N_DB, int dim);
Vector *inicializar_centroides(Vector *ELEMS, int N_DB, int dim);

float calcular_distancia(float *punto_1, float *centroide, int dim);

int centroide_mas_cercano(float *punto, int dim_pto, Vector *centroides, int num_centroides);
void recalcular_centroides(Vector *centroides, int *idx_centroides_por_punto, Vector *ELEMS, int num_centroides, int num_puntos, int dim);

int comparar_centroides(Vector *centroides, Vector *centroides_anteriores, int num_centroides, int dim);

void mostrar_indices(int *idx_centroides_por_punto, int N_DB);

int main()
{
    int N_DB, i, j, k = 0;
    Vector *ELEMS, *centroides;
    struct timeval t1, t2;
    float real_time;

    omp_set_num_threads(T);

    // se ingresa la cantidad de vectores
    scanf("%d", &N_DB);
    // creacion de matriz
    ELEMS = (Vector *)malloc(N_DB * sizeof(Vector));

    for (i = 0; i < N_DB; i++)
    {
        Vector v;
        v.data = (float *)malloc(sizeof(float) * DIM);
        ELEMS[i] = v;
    }

    // rellenar matriz
    for (i = 0; i < N_DB; i++)
    {
        float *temp_data = ELEMS[i].data;
        for (j = 0; j < DIM; j++)
        {
            scanf("%f", &temp_data[j]);
        }
    }

    gettimeofday(&t1, 0);
    #pragma omp single
    {
        centroides = inicializar_centroides(ELEMS, N_DB, DIM);
    }

    Vector *centroides_anteriores = (Vector *)malloc(K * sizeof(Vector));
    // copiando los valores de los centroides a los centroides anteriores
    for (int i = 0; i < K; i++)
    {
        Vector v;
        v.data = (float *)malloc(sizeof(float) * DIM);
        memcpy(v.data, centroides[i].data, DIM * sizeof(float));
        centroides_anteriores[i] = v;
    }

    int* idx_centroides_por_punto = (int*)calloc(N_DB,sizeof(int));// esto puede ser una funcion para mostrar lo que tengo que mostrar en pantalla
    while(1){
        #pragma omp parallel shared(N_DB, ELEMS, centroides, idx_centroides_por_punto)
        {
            int tid = omp_get_thread_num();
            
            for (int i = tid; i < N_DB; i += T) {
                float* punto = ELEMS[i].data;
                int indice_centroide_cercano = centroide_mas_cercano(punto, DIM, centroides, K);
                idx_centroides_por_punto[i] = indice_centroide_cercano; 
            }

        }
        recalcular_centroides(centroides, idx_centroides_por_punto, ELEMS, K, N_DB, DIM);
        if (comparar_centroides(centroides, centroides_anteriores, K, DIM) == 1){
            mostrar_indices(idx_centroides_por_punto, N_DB);
            break;
        }
        
        for (int i = 0; i < K; i++) {
            memcpy(centroides_anteriores[i].data, centroides[i].data, DIM*sizeof(float));
        }
        k++;
    }

    //mostrar_indices(idx_centroides_por_punto, N_DB);
    //printf("centroides finales\n");
    //mostrar_data(centroides, K, DIM);



    printf("Iteraciones: %d", k);
    gettimeofday(&t2, 0);
    real_time = (t2.tv_sec - t1.tv_sec) + (float)(t2.tv_usec - t1.tv_usec) / 1000000;
    printf("\nTiempo Real = %f\n", real_time);

    return 0;
}

Vector *inicializar_centroides(Vector *ELEMS, int N_DB, int dim)
{
    Vector *centroides = (Vector *)malloc(K * sizeof(Vector));
    Vector centroide_elegido;
    int centroide_idx, l = 0;
    srand(time(NULL));

    for (int i = 0; i < K; i++)
    {
        centroide_idx = rand() % N_DB;
        centroide_elegido.data = (float *)malloc(sizeof(float) * dim);
        memcpy(centroide_elegido.data, ELEMS[centroide_idx].data, dim * sizeof(float));
        // centroide_elegido = ELEMS[centroide_idx];
        centroides[i] = centroide_elegido;
    }
    return centroides;
}

void mostrar_data(Vector *vector, int N_DB, int dim)
{
    for (int i = 0; i < N_DB; i++)
    {
        float *temp_data = vector[i].data;
        printf("%d -> ", i);
        for (int j = 0; j < dim; j++)
        {
            printf("%.3f ", temp_data[j]);
        }
        printf("\n");
    }
}

void mostrar_indices(int *idx_centroides_por_punto, int N_DB)
{
    for (int i = 0; i < N_DB; i++)
    {
        printf("%d\n", idx_centroides_por_punto[i] + 1);
    }
}

float calcular_distancia(float *punto_1, float *centroide, int dim)
{
    float distancia = 0;
    float diff = 0;
    for (int i = 0; i < dim; i++)
    {
        diff += powf(punto_1[i] - centroide[i], 2.0);
    }
    distancia = sqrtf(diff);
    return distancia;
}

int centroide_mas_cercano(float *punto, int dim_pto, Vector *centroides, int num_centroides)
{
    // separar por hilos y guardar el centroide mas cercano elegido por cada hilo,
    //  
    int idx_centroide_mas_cercano = 0;
    float minima_distancia = INFINITY;
    for (int i = 0; i < num_centroides; i++)
    {
        float distancia_temp = calcular_distancia(punto, centroides[i].data, dim_pto);
        if (distancia_temp < minima_distancia)
        {
            minima_distancia = distancia_temp;
            idx_centroide_mas_cercano = i;
        }
    }
    return idx_centroide_mas_cercano;
}

void recalcular_centroides(Vector *centroides, int *idx_centroides_por_punto, Vector *ELEMS, int num_centroides, int num_puntos, int dim)
{
    // encontrar los puntos que pertenecen a cada centroide y calcular la media de sus coordenadas(esta media sera el nuevo centroide)
    #pragma omp parallel for
    for (int i = 0; i < num_centroides; i++)
    {
        float *sum_puntos = (float *)calloc(dim, sizeof(float));
        // memset(centroides[i].data, 0, dim*sizeof(float));

        int count = 0; // cantidad de puntos que pertenecen al centroide i
        
        #pragma omp parallel for reduction(+:count)
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

        if (count != 0)
        {
            for (int j = 0; j < dim; j++)
            {
                centroides[i].data[j] = sum_puntos[j] / count;
                // centroides[i].data[j] /= count;
            }
        }
        free(sum_puntos);
    }
}

int comparar_centroides(Vector *centroides, Vector *centroides_anteriores, int num_centroides, int dim)
{
    int iguales = 1;
    for (int i = 0; i < num_centroides; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (centroides[i].data[j] != centroides_anteriores[i].data[j])
            {
                iguales = 0;
                break;
            }
        }
    }
    return iguales;
}
