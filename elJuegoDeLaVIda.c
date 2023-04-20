// mpicc file.c -o file
// mpirun -np 4 ./file

#include <stdio.h>
#include <mpi.h>


int main(int argc, char *argv[]) {
    if (argc == 2) {
        int n = atoi(argv[1]);

        int grid[][] = (int **)malloc(n * sizeof(int *));

    } else {
        printf("Error: uso %s <n>\n", argv[0]);
        return -1;
    }
return 0;
}