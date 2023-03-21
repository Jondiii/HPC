// Compilar: mpicc file.c -o file
// Ejecutar: mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int rank, size;
  int msg = 1;
  
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  if (rank == 0) {
    printf("Soy 0 y estoy mandando el mensaje %d\n", msg);
    MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&msg, 1, MPI_INT, size-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("Soy 0 y he recibido el mensaje %d\n", msg);
  } else {
    MPI_Recv(&msg, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    printf("Soy %d y he recibido el mensaje %d\n", rank, msg);
    msg++;
    printf("Soy %d y estoy mandando el mensaje %d\n", rank, msg);
    MPI_Send(&msg, 1, MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();

  return 0;
}
