// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size;
   int numeros[100];
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   int recibidos[100 / size];

   if (rank == 0) {
      for (int i = 0; i < 100; i++)
      {
         numeros[i] = i+1;
      }
   }

   MPI_Scatter(&numeros, 100 / size, MPI_INT, &recibidos, 100 / size, MPI_INT, 0, MPI_COMM_WORLD); // NO hace falta un receive. EL 0 indica el origen del mensaje

   if (100 % size != 0) {
      //rank = 0 hace estos casos
   }

   for (int i = 0; i < 100 / size; i++)
   {
      printf("Soy el proceso %d y en la posición %d tengo %d\n", rank, i, recibidos[i]);
   }
   

   MPI_Finalize();
return 0;
}