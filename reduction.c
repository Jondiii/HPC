// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size;
   int sumatorio;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   int fragmento[100 / size];
   int total = 0;

   for (int i = 0; i < 100 / size; i++)
   {
      total += i+1 + rank * (100 / size); // Cada proceso genera un array de 100/size números. El rango 0 tendrá los N_size primeros, etc
   }

   MPI_Reduce(&total, &sumatorio, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 


   if (100 % size != 0) {
      //rank = 0 hace estos casos
      for (int i = 0; i < 100 % size; i++) {
        //fragmento[99-i] = 100 - i;
      }
   }

   // Solo se entera el proceso 0 del reduce porque es el único que la realiza.
   printf("[Reduce] Soy rank %d, total es %d sumatorio %d\n", rank, total, sumatorio);

   // Con allreduce todos suman lo de todos
   MPI_Allreduce(&total, &sumatorio, 1, MPI_INT, MPI_SUM,  MPI_COMM_WORLD); 
   
   // Solo se entera el proceso 0 del reduce porque es el único que la realiza.
   printf("[AllReduce] Soy rank %d, total es %d sumatorio %d\n", rank, total, sumatorio);

   MPI_Finalize();

return 0;
}