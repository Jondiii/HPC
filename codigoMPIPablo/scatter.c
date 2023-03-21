// Compilar: mpicc file.c -o file
// Ejecutar: mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int rank, size;
  int numeros[100];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int recibidos[100/size];
    
  if (rank == 0) {
    for (int i = 0; i < 100; i++) {
      numeros[i] = i+1;
    }
  }
  
  MPI_Scatter(&numeros, 100 / size, MPI_INT, &recibidos, 100 / size, MPI_INT, 0, MPI_COMM_WORLD);
  
  if (100 % size != 0) {
    // Hacer que rank==0 haga estos casos
  }
  
  for (int i = 0; i < 100 / size; i++) {
    printf("Soy el proceso %d y recibidos[%d] = %d\n", rank, i, recibidos[i]);
  }
  
  MPI_Finalize();

  return 0;
}
