#include <omp.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>

#define SIZE 32
#define ALPHA "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define MIN 1
#define MAX 6
#define MAXCANDIDATE 256
#define HASHSIZE 64
#define CHUNK 1000000
// #define SALT 0

int compute_hash(char *str, unsigned char mdString[SIZE * 2 + 1])
{
  unsigned char digest[SIZE];
  SHA256_CTX ctx;
  int i;

  SHA256_Init(&ctx);
  SHA256_Update(&ctx, str, strlen(str));
  SHA256_Final(digest, &ctx);

  for (i = 0; i < SIZE; i++)
    sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]); // Añadir posible salt creo que sería con Update (SHA256_Update())
  mdString[SIZE * 2] = '\0';

  return 0;
}

void next_candidate(char *candidate, char *alpha)
{
  int done = 0;
  for (int i = strlen(candidate) - 1; i >= 0; i--)
  {
    int pos = (int)(strchr(alpha, candidate[i]) - alpha);
    if (pos != strlen(alpha) - 1)
    {
      candidate[i] = alpha[pos + 1];
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
    sprintf(candidate, "%c%s", alpha[0], candidate);
  }
}

int main(int argc, char **argv)
{
  char *avalue = NULL;
  char *dvalue = NULL;
  char *mvalue = NULL;
  char *nvalue = NULL;
  // char *svalue = NULL; //value for salt
  char secreto[HASHSIZE + 1];
  char hashString[HASHSIZE + 1];
  char candidate[MAXCANDIDATE];
  int n, m, c, index;
  int found = 0;

  opterr = 0;

  while ((c = getopt(argc, argv, "a:d:m:n:")) != -1)
    switch (c)
    {
    case 'a':
      avalue = optarg;
      break;
    case 'd':
      dvalue = optarg;
      break;
    case 'm':
      mvalue = optarg;
      break;
    case 'n':
      nvalue = optarg;
      break;
      //    case 's':
      //      svalue = optarg;
      //    break;
    case '?':
      if (optopt == 'a' || optopt == 'd' || optopt == 'm' || optopt == 'n' /*|| optopt == 's'*/)
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort();
    }

  if (mvalue != NULL)
  {
    m = atoi(mvalue);
  }
  else
  {
    m = MAX;
  }

  if (nvalue != NULL)
  {
    n = atoi(nvalue);
  }
  else
  {
    n = MIN;
  }
  /*
  if (svalue != NULL)
  {
    s = atoi(svalue);
  }
  else
  {
    s = SALT;
  }
  */

  if (avalue == NULL)
  {
    avalue = (char *)malloc(sizeof(char) * strlen(ALPHA));
    sprintf(avalue, "%s", ALPHA);
  }

  printf("diccionario = %s, alfabeto = %s, max = %d min = %d \n",
         dvalue, avalue, m, n); // añadir salt

  if (argv[optind] != NULL)
  {
    sprintf(secreto, "%s", argv[optind]);
    printf("Hash: %s\n", secreto);
    if (dvalue != NULL)
    {
      FILE *dict;
      dict = fopen(dvalue, "r");
      if (dict != NULL)
      {
        int total = 0;
        int cracked = 0;
        while (fscanf(dict, "%s\n", candidate) == 1)
        {
          total++;
        }
        rewind(dict);

#pragma omp parallel for shared(found) private(candidate, hashString) reduction(+: cracked)
        for (int i = 0; i < total; i++)
        {
          if (!found)
          {
            fscanf(dict, "%s\n", candidate);
            compute_hash(candidate, hashString);
            cracked++;
            if (strcmp(hashString, secreto) == 0)
            {
              printf("PASSWORD FOUND!\n");
              printf("SHA256(%s) = %s\n", candidate, secreto);
              found = 1;
            }
          }
        }
        printf("Keyspace size: %d, Total passwords cracked: %d\n", total, cracked);
        fclose(dict);
        if (!found)
          printf("Password not found in file %s!\n", dvalue);
      }
      else
      {
        printf("Error: unable to open dict file!\n");
        return 1;
      }
    }
    

    // Ataque a fuerza bruta
    if (!found) {
      char candidates[CHUNK][m + 1];

      sprintf(candidate, "%c", avalue[0]);

      while (strlen(candidate) <= m && !found) {
        
        #pragma omp parallel for private(hashString) shared(found, candidate)
        for (int i = 0; i < CHUNK; i++) {
          sprintf(candidates[i], "%s", candidate);
          next_candidate(candidate, ALPHA); //HACER QUE EN VEZ DE next (+1) sea +N_procesos.
                                            //Inicializar un array de candidatas que tengan también estos saltos.

          if (!found) {
            compute_hash(candidates[i], hashString);

            if (strcmp(hashString, secreto) == 0) {
              printf("PASSWORD FOUND!\n");
              
              printf("SHA256(%s) = %s\n", candidates[i], secreto);
              
              found = 1;
              #pragma omp flush(found)
            }
          }
        }
      }
    }
  }
  else
  {
    printf("Error: hash not found!\n");
    return 1;
  }
  return 0;
}
