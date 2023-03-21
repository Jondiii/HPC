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

  int enviados[100/size];
    
  for (int i = 0; i < 100/size; i++) {
      enviados[i] = i+1 + rank*100/size;
  }
  
  MPI_Gather(&enviados, 100 / size, MPI_INT, &numeros, 100 / size, MPI_INT, 0, MPI_COMM_WORLD);
  
  if (100 % size != 0) {
    // Hacer que rank==0 haga estos casos
    for (int i = 0; i < 100 % size; i++) {
    	numeros[99-i] = 100 - i;
    }
  }

  if (rank == 0) {
    for (int i = 0; i < 100; i++) {
      printf("numeros[%d] = %d\n", i, numeros[i]);
    }
  }
  
  MPI_Finalize();

  return 0;
}
