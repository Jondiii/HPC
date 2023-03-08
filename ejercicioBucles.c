#include <omp.h>
#include <stdio.h>

#define SIZE 100000

int main(int argc, char const *argv[])
{
    int a[SIZE];

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++)
    {
        a[i] = i;
    }
    

    return 0;
}
