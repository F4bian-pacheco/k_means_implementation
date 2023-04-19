# Repositorio de Implementación Secuencial y Paralela del Algoritmo K-means en C
Este repositorio contiene una implementación del algoritmo K-means tanto en su versión secuencial como en su versión paralela utilizando OpenMP. El objetivo de este proyecto es proporcionar una comparación de rendimiento entre ambas implementaciones y permitir a los usuarios evaluar cuál es la mejor opción según sus necesidades.

## ¿Qué es K-means?
El algoritmo K-means es un método de agrupamiento que se utiliza para clasificar un conjunto de datos en K grupos distintos. El objetivo del algoritmo es minimizar la varianza dentro de cada grupo y maximizar la distancia entre los grupos. El proceso implica asignar aleatoriamente K centroides iniciales y luego ajustar su posición iterativamente hasta que se logre la convergencia.

## Estructura del repositorio
El repositorio contiene dos carpetas principales:

* sequential: esta carpeta contiene la implementación secuencial del algoritmo K-means en C.
* parallel: esta carpeta contiene la implementación paralela del algoritmo K-means utilizando OpenMP en C.
Además, el repositorio contiene un archivo README.md que proporciona una guía para compilar y ejecutar ambas implementaciones.
* tests: carpeta donde se encuentran test de datos

## Cómo usar el repositorio
Para usar este repositorio, siga estos pasos:

1. Clone el repositorio en su máquina local.
2. Abra una terminal y vaya a la carpeta "sequential" o "parallel", según la implementación que desee utilizar.
3. Compile el código utilizando el siguiente comando: gcc programa.c -o programa -lm si entra a la carpeta "sequential"
  * use gcc programa_p.c -o programa_p -fopenmp -lm, si entra a la carpeta "parallel"
5. Ejecute el archivo generado con el comando ./programa < ../tests/Test_BD_95000.txt. El programa leerá los datos del archivo y realizará el agrupamiento utilizando el algoritmo K-means.
6. La salida del programa será una lista de números que corresponden al centroide al que pertenece cada punto del archivo de datos.
