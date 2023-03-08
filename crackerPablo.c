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
#define MIN 1
#define MAX 8
#define MAXCANDIDATE 256
#define HASHSIZE 32

int compute_hash(char *str, unsigned char mdString[65]) {
  unsigned char digest[SIZE];
  SHA256_CTX ctx;
  int i;

  SHA256_Init(&ctx);
  SHA256_Update(&ctx, str, strlen(str));
  SHA224_Final(digest, &ctx);

  for (i = 0; i < SIZE; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);//sprintf imprime dentro del script

  return 0;
}

int main (int argc, char **argv) {
  char *avalue = NULL;
  char *dvalue = NULL;
  char *mvalue = NULL;
  char *nvalue = NULL;
  char secreto[HASHSIZE+1];
  char hashString[HASHSIZE+1];
  char candidata[MAXCANDIDATE];
  int n, m;
  int index;
  int c;
  int found = 0;

  opterr = 0;

  while ((c = getopt (argc, argv, "a:d:m:n:")) != -1)
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
      case '?':
        if (optopt == 'a' || optopt == 'd' || optopt == 'm' || optopt == 'n') 
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        abort ();
      }

  if (mvalue != NULL) {      
    m = atoi(mvalue);
  } else {
    m = MAX;
  }
  
  if (nvalue != NULL) {
    n = atoi(nvalue);
  } else {
    n = MIN;
  }

  if (avalue == NULL) {
    avalue = (char *) malloc(sizeof(char) * strlen(ALPHA));
    sprintf(avalue, "%s", ALPHA);
  } 

  printf ("diccionario = %s, alfabeto = %s, max = %d min = %d \n",
          dvalue, avalue, m, n);

  if (argv[optind] != NULL) {
    sprintf(secreto, "%s", argv[optind]);
    printf ("Hash: %s\n", secreto);
    
    // ¿Hay que hacer un ataque de diccionario?
    if (dvalue != NULL) {
      // Sí, cracking por diccionario
      FILE *file;
      file = fopen(dvalue, "r");
      if (file != NULL) {
        // Leemos las claves candidatas y crackeamos...
        int total = 0;
        int cracked = 0;
        while (fscanf(file, "%s\n", candidata) == 1) {
          total++;
        } 
        rewind(file);
        #pragma omp parallel for shared(found) private(candidata, hashString) reduction(+: cracked)
        for (int i = 0; i < total; i++) {
          if (!found) {
            fscanf(file, "%s\n", candidata);
            compute_hash(candidata, hashString);
            cracked++;
            if (strcmp(hashString, secreto) == 0) {
              printf("PASSWORD FOUND!\n");
              printf("md5(%s) = %s\n", candidata, secreto);
              found = 1;
            }
          }
        }
        
        printf("Keyspace size: %d, Total passwords cracked: %d\n", total, cracked);
     
        fclose(file);
        if (!found) printf("Password not found in file %s!\n", dvalue);
      } else {
        printf("Error: unable to open dict file!\n");
        return 1;
      }
    }
    // Si todavía no hemos encontrado la clave, ataque por fuerza bruta...
    /*
    if (!found) {
      sprintf(candidata, "%c", avalue[0]); // -> Inicializar mejor, teniendo en cuenta n
      while(strlen(candidata) <= m && !found) { // -> Esto debería ser un for -> omp
        compute_hash(candidata, hashString);
        if (strcmp(hashString, secreto) == 0) {
              printf("PASSWORD FOUND!\n");
              printf("md5(%s) = %s\n", candidata, secreto);
              found = 1;
        }
        candidata = next(candidata, avalue); // -> Programar next()
      }
    }*/
    
  } else {
    printf("Error: hash not found!\n");
    return 1;
  }
  
  return 0;
}

