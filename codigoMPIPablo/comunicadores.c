// Compilar: mpicc file.c -o file
// Ejecutar: mpirun -np 4 ./file

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 10000

int main(int argc, char *argv[]) {
  int rank, size, rank_subgrupo;
  int numeros[SIZE];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size < 10) {
    printf("Please run this application with at least 10 processes.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  for (int i = 0; i < SIZE; i++) {
    numeros[i] = rank;
  }
  
  MPI_Group world_group, pares_group, impares_group;
  MPI_Comm_group(MPI_COMM_WORLD, &world_group);
  
  int rango_pares[5] = {0, 2, 4, 6, 8};
  int rango_impares[6] = {1, 3, 5, 7, 9, 0};

  MPI_Group_incl(world_group, 5, rango_pares, &pares_group);
  MPI_Group_incl(world_group, 6, rango_impares, &impares_group);
  
  MPI_Comm pares_comm, impares_comm;
  MPI_Comm_create(MPI_COMM_WORLD, pares_group, &pares_comm);
  MPI_Comm_create(MPI_COMM_WORLD, impares_group, &impares_comm);
  
  if (rank % 2 == 0) {
    MPI_Comm_rank(pares_comm, &rank_subgrupo);
  } else {
    MPI_Comm_rank(impares_comm, &rank_subgrupo);
  }
  
  if (pares_comm != MPI_COMM_NULL) {
    MPI_Bcast(&numeros, SIZE, MPI_INT, 0, pares_comm);
  } 
  if (impares_comm != MPI_COMM_NULL) {
    MPI_Bcast(&numeros, SIZE, MPI_INT, 0, impares_comm);
  }

  printf("Soy el proceso %d en MPI_COMM_WORLD, pero en mi subgrupo soy %d y para mí numeros[0] = %d\n", rank, rank_subgrupo, numeros[0]);

  MPI_Finalize();

  return 0;
}
