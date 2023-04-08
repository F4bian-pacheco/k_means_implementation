#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* Estructura para almacenar un vector */
typedef struct {
    int n;          /* Dimension del vector */
    double* data;   /* Coordenadas del vector */
} Vector;

Vector read_vector(int n);
double distance(Vector v1, Vector v2);
Vector* init_centroids(Vector* data, int DIM, int n, int k);
int* assign_clusters(Vector* data, int n, Vector* centroids, int k);
void update_centroids(Vector* data, int n, int* clusters, Vector* centroids, int k);
void free_vectors(Vector* data, int n, Vector* centroids, int k);

int main() {
    int DIM,n,k;
    printf("Numero de vectores: ");
    scanf("%d", &n); // dimendion del vector
    printf("Numero de elementos por vector: ");
    scanf("%d", &DIM);
    printf("Numero de clusters: ");
    scanf("%d", &k); // numero de clusters
    /* Leer los vectores */
    Vector* data = (Vector*)malloc(n * sizeof(Vector));
    for (int i = 0; i < n; i++) {
        data[i] = read_vector(DIM);
    }

    /* Inicializar los centroides al azar */
    Vector* centroids = init_centroids(data, DIM, n, k);

    /* Iterar hasta que la asignacion de clusters no cambie */
    int* old_clusters = (int*)malloc(n * sizeof(int));
    int* new_clusters = assign_clusters(data, n, centroids, k);
    while (1) {
        /* Actualizar los centroides */
        update_centroids(data, n, new_clusters, centroids, k);

        /* Asignar los vectores al centroide mas cercano */
        old_clusters = new_clusters;
        new_clusters = assign_clusters(data, n, centroids, k);

        /* Verificar si la asignacion de clusters ha cambiado */
        int has_changed = 0;
        for (int i = 0; i < n; i++) {
            if (old_clusters[i] != new_clusters[i]) {
                has_changed = 1;
                break;
            }
        }
        if (!has_changed) {
            break;
        }
    }

    /* Imprimir los numeros de cluster asignados a cada elemento */
    for (int i = 0; i < n; i++) {
        printf("%d\n", new_clusters[i]);
    }

    /* Liberar la memoria */
    //free_vectors(data, n, centroids, k);
    free(old_clusters);
    free(new_clusters);

    return 0;
}



/* Funcion para leer un vector desde la entrada */
Vector read_vector(int dim) {
    Vector v;
    v.n = dim;
    v.data = (double*)malloc(dim * sizeof(double));
    for (int i = 0; i < dim; i++) {
        scanf("%lf", &v.data[i]);
    }
    return v;
}

/* Funcion para calcular la distancia euclidiana entre dos vectores */
double distance(Vector v1, Vector v2) {
    double sum = 0.0;
    for (int i = 0; i < v1.n; i++) {
        sum += pow(v1.data[i] - v2.data[i], 2);
    }
    return sqrt(sum);
}

/* Funcion para inicializar los centroides al azar */
Vector* init_centroids(Vector* data, int dim, int n, int k) {
    Vector* centroids = (Vector*)malloc(k * sizeof(Vector));
    centroids[0] = data[0];
    for (int i = 1; i < k; i++) {
        int j = rand() % dim;
        centroids[i] = data[j];
    }
    return centroids;
}

/* Funcion para asignar cada vector al centroide mas cercano */ //retorna la distancia pero debe guardar un punto
int* assign_clusters(Vector* data, int n, Vector* centroids, int k) {
    int* clusters = (int*)malloc(n * sizeof(int)); // vector de cluster [d_cent1, d_cent2,...,d_centk]
    for (int i = 0; i < n; i++) {
        double min_dist = distance(data[i], centroids[0]);
        int min_idx = 0;
        for (int j = 1; j < k; j++) {
            double d = distance(data[i], centroids[j]);
            if (d < min_dist) {
                min_dist = d;
                min_idx = j;
            }
        }
        clusters[i] = min_idx;
    }
    return clusters;
}

/* Funcion para actualizar los centroides */
void update_centroids(Vector* data, int n, int* clusters, Vector* centroids, int k) {
    int* counts = (int*)calloc(k, sizeof(int));
    for (int i = 0; i < n; i++) {
        int j = clusters[i];
        counts[j]++;
        for (int l = 0; l < centroids[j].n; l++) {
            centroids[j].data[l] += data[i].data[l];
        }
    }
    for (int i = 0; i < k; i++) {
        for (int l = 0; l < centroids[i].n; l++) {
            centroids[i].data[l] /= counts[i];
        }
    }
    free(counts);
}

/* Funcion para liberar la memoria utilizada por los vectores */
void free_vectors(Vector* data, int n, Vector* centroids, int k){
    for (int i = 0; i < n; i++) {
        free(data[i].data);
        data[i].data = NULL;
    }
    free(data);
    for (int i = 0; i < k; i++) {
        free(centroids[i].data);
        centroids[i].data = NULL;
    }
    free(centroids);
}

