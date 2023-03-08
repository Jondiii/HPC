#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
 # pragma omp paralel for // lo de paralel for es para dividir el trabajo del for, en vez de para que haga el bucle en cada procesador
 for (int i = 0; i < 1000; i++) {
    printf("Hola, mundo!\n");
 }

 return 0;
} // WC -> word count