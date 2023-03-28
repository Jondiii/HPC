// mpicc file.c -o file
// mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>

#define TIMES 100

int main(int argc, char *argv[]) {
    int rank, size;
    double t1, t2;
    double benchmark[TIMES];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Esto es una forma BÁSICA de hacer que los prints salgan ordenados.

    t1 = MPI_Wtime();
    for (int j = 0; j < TIMES; j++) {
    for (int i = 0; i < rank; i++){
        MPI_Barrier(MPI_COMM_WORLD); // Punto de sincronización (todos los procesos deben estar aquí para poder seguir)
    }

    printf("Hello world from process %d of %d\n", rank, size);

    for (int i = 0; i < size-1-rank; i++) {
        MPI_Barrier(MPI_COMM_WORLD); // Punto de sincronización (todos los procesos deben estar aquí para poder seguir)
    }

    t2 = MPI_Wtime();
    benchmark[j] = t2-t1;
    t1 = t2;

    }

    printf("Tiempo hilo %d: %f\n", rank, benchmark[1]);
    /* 
    // Otra forma de hacerlo 
    for (int i = 0; i < size; i++) {
        if (i == rank) {
            printf("Hello world from process %d of %d\n", rank, size);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }*/

    MPI_Finalize();
return 0;
}