// mpicc file.c -o file
// mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>

#define TIMES 10
#define SIZE 10

int main(int argc, char *argv[]) {
    int rank, size;
    double t1, t2;
    double buf[SIZE];
    int msg = 31337;

    MPI_Status statuses[TIMES];
    MPI_Request requests[TIMES];
    int indices[TIMES];
    int times;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < SIZE; i++) {
        buf[i] = i * i;
    }

    t1 = MPI_Wtime();
    for (int i = 0; i < TIMES; i++) {
        if (rank == 0) { //No hacer nada con Irecv/Isend hasta que recv y send funcionen bien
            MPI_Irecv(&buf, SIZE, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[i]); 
        } else {
            MPI_Isend(&buf, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &requests[i]); //Inmediato
        }
    }
    
    if (rank == 0) {
        MPI_Waitsome(TIMES, requests, &times, indices, statuses);
    }

    t2 = MPI_Wtime();

    if (rank == 0) {
        printf("Tiempo: %f\n", (t2 - t1)/TIMES);
    }


    MPI_Finalize();

return 0;
}