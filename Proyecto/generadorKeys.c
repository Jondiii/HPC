#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>

#define SIZE 32
#define ALPHA "abcde"
#define MIN 1
#define MAX 6
#define MAXCANDIDATE 256
#define HASHSIZE 64

#define CHUNK 20
#define NTHREADS 4

void next_candidate2(char *candidate, char *alpha)
{
  int done = 0;
  int newPos = 0;
  int nosPasamos = 0;
  char temp[MAXCANDIDATE];

  for (int i = strlen(candidate) - 1; i >= 0; i--)
  {
    int pos = (int)(strchr(alpha, candidate[i]) - alpha);
    // Si nos salimos del alfabeto al hacer el salto, se calcula dónde acabamos.
    if (pos + NTHREADS > strlen(alpha) -1 && !nosPasamos) {
      newPos = pos + NTHREADS - strlen(alpha);
      candidate[i] = alpha[newPos];
      nosPasamos = 1; // No se vuelve a usar nosPasamos hasta la siguiente iteración, si estamos ya en la última esto no vale
  
    } else if (pos != strlen(alpha) - 1) {
      if (nosPasamos) {
        candidate[i] = alpha[pos + 1];
        done = 1;
        break;
      }
      candidate[i] = alpha[pos + NTHREADS]; 
      done = 1;
      break;
    }
    else
    {
      candidate[i] = alpha[0];
    }
  }
  if (!done)
  {
    if (omp_get_thread_num() == 0) {
      printf("CANDIDATE ANTES not DONE: %s\n",candidate);
    }

    sprintf(temp, "%s", candidate);

    sprintf(candidate, "%c%s", alpha[0], temp);

    if (omp_get_thread_num() == 0) {
      printf("CANDIDATE LUEGO not DONE: %s\n",candidate);
    } 
  }
}

void init_candidates(char *candidates, char *alpha) {
  for (int i = 0; i < NTHREADS; i++) {
   sprintf(&candidates[i * (CHUNK/NTHREADS)], "%c", alpha[i]);
  }
}


int main(int argc, char **argv)
{   
    omp_set_num_threads(NTHREADS);
    char *avalue = NULL;
    
    avalue = (char *)malloc(sizeof(char) * strlen(ALPHA));
    sprintf(avalue, "%s", ALPHA);
  
    int m = 6; //Longitud máxima de la clave
    char secreto[HASHSIZE + 1];
    char hashString[HASHSIZE + 1];
    char candidate[MAXCANDIDATE];
    char candidates[CHUNK][m + 1];

    int firstTime = 1;

    int threadIDs[CHUNK];

    sprintf(candidates[0], "%c", avalue[0]);
    sprintf(candidates[CHUNK/NTHREADS], "%c", avalue[0]);

    for (int i = 0; i < NTHREADS; i++){
      sprintf(candidates[(CHUNK/NTHREADS) * i], "%c", avalue[i]);
      
      printf("Letra del alfabeto a introducir: %c\n",avalue[i]);
      printf("Letra del alfabeto introducida : %s\n",candidates[(CHUNK/NTHREADS) * i]);
      printf("Posición donde se ha guardado  : %d\n\n",(CHUNK/NTHREADS) * i);
    }

    // gcc -fopenmp generadorKeys.c -o 2generadorKeys
    for (int j = 0; j <= 0; j++) {
      //#pragma omp parallel for private(hashString) shared(candidate)
      #pragma omp parallel for private(candidate) firstprivate(firstTime)
      for (int i = 0; i < CHUNK; i++) {
          if (firstTime) { //Hay que inicializar la variable candidate, que es la que se usa en el cracker principal.
            sprintf(candidate, "%s", candidates[(CHUNK/NTHREADS) * omp_get_thread_num()]);
            printf("Division: %d\n",(CHUNK/NTHREADS) * omp_get_thread_num());
            firstTime = 0;
          }

          sprintf(candidates[i], "%s", candidate);
          next_candidate2(candidate, ALPHA);  // En vez de hacer +1 hace +NTHREADS en el alfabeto
          threadIDs[i] = omp_get_thread_num();
          }
      
      firstTime = 0;

      for (int i = 0; i < CHUNK; i++) {
          printf("T%d-Combinación %d: %s\n", threadIDs[i], i, candidates[i]);
      }
      printf("\n");
    }
  return 0;
}

  
  