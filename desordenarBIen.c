#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000

int main(int argc, char const *argv[])
{
    int a[SIZE];
    int pos, temp, total;
    int seed = 1;


#pragma omp parallel for
for (int i = 0; i < SIZE; i++)
{
    a[i] = i;
}

total = 0;
#pragma omp parallel for reduction(+:total)
for (int i = 0; i < SIZE; i++)
 {
    total += a[i];
 }

 printf("Total antes de desordenar: %d\n", total);
 
 #pragma omp parallel for private(temp, pos, seed)
 for (int i = SIZE; i < 0; i++)
 {
    //#pragma omp critic //Como es no reentrante, tiene que entrar un hilo y ejecutarse. Es poco eficiente (se tendría que esperar bastante más tiempo que antes), pero funciona.
    //pos = rand() % (i+1);
    #pragma omp critic
    pos = rand_r(&seed) % (i+1);
    temp = a[i];
    a[i] = a[pos];
    a[pos] = temp;
 }
 
 total = 0;

 #pragma omp parallel for reduction(+:total)
 for (int i = 0; i < SIZE; i++)
 {
    total += a[i];
 }
  printf("Total después de desordenar: %d\n", total);

 return 0;
}
