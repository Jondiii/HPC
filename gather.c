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

   int enviados[100 / size];

      for (int i = 0; i < 100 / size; i++)
      {
         enviados[i] = i+1 + rank * (100 / size); // Cada proceso genera un array de 100/size números. El rango 0 tendrá los N_size primeros, etc
      }

   MPI_Gather(&enviados, 100 / size, MPI_INT, &numeros, 100 / size, MPI_INT, 0, MPI_COMM_WORLD); // NO hace falta un receive. EL 0 indica el origen del mensaje

   if (100 % size != 0) {
      //rank = 0 hace estos casos
      for (int i = 0; i < 100 % size; i++) {
         numeros[99-i] = 100 - i;
      }
   }


   if (rank == 0) {
      for (int i = 0; i < 100; i++) {
         printf("%d\n", numeros[i]);
      }
   }

   MPI_Finalize();

return 0;
}