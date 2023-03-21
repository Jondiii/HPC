// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size, pos, temp;
   int numeros[100];
   int seed = 1;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (rank == 0){
      for (int i = 0; i < 100; i++)
      {
         numeros[i] = i + 1;
      }

      // Desordenamos los números
      for (int i = 99; i >= 0; i--)
      {
         pos = rand() % (i+1);
         temp = numeros[pos];
         numeros[pos] = numeros[i];
         numeros[i] = temp;
      }      
   }

   MPI_Bcast(&numeros, 100, MPI_INT, 0, MPI_COMM_WORLD);
   
   for (int i = 0; i < 100; i++)
   {
      printf("Soy el proceso %d y numeros[%d] = %d\n", rank, i, numeros[i]);
   }
   
   MPI_Finalize();

return 0;
}