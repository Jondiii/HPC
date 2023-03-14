// mpicc file.c -o file
// mpirun -np 4 ./file
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
   int rank, size;
   int msg = 0;
   int buf;

   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (rank == 0) {
      printf("Manager: esperando el mensaje...\n");
      while (msg < 100){
         for (int i = 1; i < size; i++){
            MPI_Send(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Manager: mandando mensaje...  \n");
            MPI_Recv(&msg, 1, MPI_INT,  i, 0, MPI_COMM_WORLD, &status); //MPI_ANY_SOURCE
            printf("Mensaje: %d\n", msg);
         }

      }
   } else {
      while (msg < 100) {
         printf("Worker %d: recibiendo mensaje...\n", rank);
         MPI_Recv(&msg, 1, MPI_INT,  0, 0, MPI_COMM_WORLD, &status); //MPI_ANY_SOURCE
         msg++;
         printf("Worker %d: mandando mensaje...\n", rank);
         MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
   }
   
   MPI_Finalize();
return 0;
}