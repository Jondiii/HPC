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
    msg = 10000;
  }
  
  MPI_Bcast(&msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  printf("Soy el proceso %d y he recibido el msg = %d\n", rank, msg);
  
  MPI_Finalize();

  return 0;
}
