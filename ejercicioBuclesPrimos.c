#include <omp.h>
#include <stdio.h>

#define SIZE 100000

int es_primo(int n) {
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char const *argv[])
{
    int a[SIZE];

    #pragma omp parallel for if (SIZE > 50000)
    for (int i = 0; i < SIZE; i++)
    {
        a[i] = es_primo(i);
    }
        
    int temp;

    #pragma omp parallel for if (SIZE > 50000)
    for (int i = 0; i < SIZE; i++)
    {
        temp = a[i];
        a[i] = a[SIZE - 1 - i];
        a[SIZE - 1 - i] = temp;
    }
    
    

    return 0;
}
