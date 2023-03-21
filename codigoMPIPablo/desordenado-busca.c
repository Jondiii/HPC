// Compilar: mpicc file.c -o file
// Ejecutar: mpirun -np 4 ./file

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 10000

int main(int argc, char *argv[]) {
  int rank, size;
  int numeros[SIZE];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    for (int i = 0; i < SIZE; i++) {
      numeros[i] = i+1;
    }
    int pos, tmp;
    for (int i = SIZE-1; i >= 0; i--) {
      pos = rand() % (i + 1);
      tmp = numeros[pos];
      numeros[pos] = numeros[i];
      numeros[i] = tmp;
    }
  }
  
  int fragmento[SIZE/size];
  
  MPI_Scatter(&numeros, SIZE/size, MPI_INT, &fragmento, SIZE/size, MPI_INT, 0, MPI_COMM_WORLD);
  
  int found = 0;
  
  for (int i = 0; i < SIZE/size; i++) {
    // algo nuy pesado de calcular
    if (fragmento[i] == 1337) {
      printf("Soy %d y he encontrado el número, fragmento[%d] = %d\n", rank, i, 1337);
      found = 1;
    }
    if (i % 10000 == 0) {
      MPI_Allreduce(&found, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
      if (found) {
        break;
      }
    }
  }
  
  MPI_Finalize();

  return 0;
}
