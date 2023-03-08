#include <omp.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
#pragma omp parallel // Lo de pragma son para dar indicaciones durante la compilación. Estamos diciendo que la siguiente linea/bloque se compila en paralelo.
{
    printf("Hello, world!\n");
    printf("Bye, world!\n");
}
} // El código se puede compilar en secuencial y en paralelo.
// gcc holaParalelo.c -o holaParalelo-seuencial -> Saca un único hola y adios
// gcc -fopenmp holaParalelo.c -o holaParalelo-paralelo -> Saca tantos hola y adios mundo como procesadores tenga el pc