// mpicc file.c -o file
// mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>

#define TIMES 1000

int main(int argc, char *argv[]) {
    int rank, size;
    double t1, t2;
    double buf[TIMES];
    int msg = 31337;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int i = 0; i < 1000; i++) {
        buf[i] = i * i;
    }

    t1 = MPI_Wtime();
    for (int i = 0; i < TIMES; i++) {
        MPI_Bcast(&buf, 1000, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    t2 = MPI_Wtime();

    if (rank == 0) {
        printf("Tiempo: %f\n", (t2 - t1)/TIMES);
    }


    MPI_Finalize();

return 0;
}