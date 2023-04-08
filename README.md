# Repositorio de Implementación Secuencial y Paralela del Algoritmo K-means en C
Este repositorio contiene una implementación del algoritmo K-means tanto en su versión secuencial como en su versión paralela utilizando OpenMP. El objetivo de este proyecto es proporcionar una comparación de rendimiento entre ambas implementaciones y permitir a los usuarios evaluar cuál es la mejor opción según sus necesidades.

## ¿Qué es K-means?
El algoritmo K-means es un método de agrupamiento que se utiliza para clasificar un conjunto de datos en K grupos distintos. El objetivo del algoritmo es minimizar la varianza dentro de cada grupo y maximizar la distancia entre los grupos. El proceso implica asignar aleatoriamente K centroides iniciales y luego ajustar su posición iterativamente hasta que se logre la convergencia.

## Estructura del repositorio
El repositorio contiene dos carpetas principales:

* sequential: esta carpeta contiene la implementación secuencial del algoritmo K-means en C.
* parallel: esta carpeta contiene la implementación paralela del algoritmo K-means utilizando OpenMP en C.
Además, el repositorio contiene un archivo README.md que proporciona una guía para compilar y ejecutar ambas implementaciones.

## Cómo usar el repositorio
Para usar este repositorio, siga estos pasos:

1. Clone el repositorio en su máquina local.
2. Abra una terminal y vaya a la carpeta "sequential" o "parallel", según la implementación que desee utilizar.
3. Compile el código utilizando el siguiente comando: gcc kmeans.c -o kmeans -lm
4. Ejecute el archivo generado con el comando ./kmeans < data.txt. El programa leerá los datos del archivo data.txt y realizará el agrupamiento utilizando el algoritmo K-means.
5. La salida del programa será una lista de números que corresponden al centroide al que pertenece cada punto del archivo data.txt.
