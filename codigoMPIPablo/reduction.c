// Compilar: mpicc file.c -o file
// Ejecutar: mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
  int rank, size;
  int sumatorio;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int total = 0;
    
  for (int i = 0; i < 100/size; i++) {
      total += i+1 + rank*100/size;
  }
  
  MPI_Allreduce(&total, &sumatorio, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
  if (100 % size != 0) {
    // Hacer que rank==0 haga estos casos
    for (int i = 0; i < 100 % size; i++) {
    	
    }
  }

  printf("Soy rank %d y total = %d y sumatorio = %d\n", rank, total, sumatorio);
  
  MPI_Finalize();

  return 0;
}
