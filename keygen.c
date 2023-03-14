// compile: gcc cracker.c -o cracker -lcrypto
#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>

#define SIZE 32
#define ALPHA "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define CHUNK 10000

void next(char* candidate, char*alpha) {
  int done = 0;
  for (int i = strlen(candidate) - 1; i >= 0 ; i--)
  {
    int pos = (int)(strchr(alpha, candidate[i]) - alpha); //Sacamos la posición de la letra en el alfabeto que tenemos
    if (pos != strlen(alpha) - 1) {
      candidate[i] = alpha[pos+1];
      done = 1;
      break;
    } else {
      candidate[i] = alpha[0];
    }
  if (!done) { // Si no hemos encontrado la clave, 
    sprintf(candidate, "%c%s", alpha[0], candidate);
  }
  }
}

// Todo lo del main se reserva en memoria antes de la ejecución del programa.
int main(int argc, char const *argv[])
{
  char candidate[SIZE];
  char candidates[CHUNK][SIZE];

  memset(candidate, 0, SIZE);
  candidate[0] = ALPHA[0];

  for (int i = 0; i < CHUNK; i++)
  {
      sprintf(candidates[i], "%s", candidate);
      next(candidate, ALPHA);
  }
  
  for (int i = 0; i < CHUNK; i++)
  {
    printf("%s\n", candidates[i]);

  }
  
  return 0;
}
