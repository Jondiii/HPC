// mpicc crackerPabloMPI.c -o cracker -lcrypto
#include <mpi.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/md5.h>

#define SIZE 16
#define ALPHA "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define MIN 1
#define MAX 8
#define MAXCANDIDATE 256
#define HASHSIZE 32
#define CHUNK 10000

void next (char *candidate, char *alpha) {
  int done = 0;
  for (int i = strlen(candidate) - 1; i >=0; i--) {
    int pos = (int) (strchr(alpha, candidate[i]) - alpha);
    if (pos != strlen(alpha) - 1) {
      candidate[i] = alpha[pos+1];
      done = 1;
      break;
    } else {
      candidate[i] = alpha[0];
    }
  }
  if (!done) {
    sprintf(candidate, "%c%s", alpha[0], candidate);
  }
}

int compute_hash(char *str, unsigned char mdString[SIZE*2+1]) {
  unsigned char digest[SIZE];
  MD5_CTX ctx;
  int i;

  MD5_Init(&ctx);
  MD5_Update(&ctx, str, strlen(str));
  MD5_Final(digest, &ctx);

  for (i = 0; i < SIZE; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
  mdString[SIZE*2] = '\0';

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
  int newFound = 0;

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


  int rank, size;

  if (argv[optind] != NULL) {
    sprintf(secreto, "%s", argv[optind]);
    
    if (rank == 0) {
      printf ("Hash: %s\n", secreto);
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // ¿Hay que hacer un ataque de diccionario?
    if (dvalue != NULL) {
    // Sí, cracking por diccionario
  
      int total = 0;
      FILE *file;

      if (rank == 0) { // Rank 0 calcula el total
        file = fopen(dvalue, "r");
        if (file != NULL) {
          // Leemos las claves candidatas y crackeamos...
          while (fscanf(file, "%s\n", candidata) == 1) {
            total++;
            fclose(file);
          }
        }
      } // IGUAL SOBRA

      MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
      
      char candidatas_dict[total][m+1];
      char mis_candidatas_dict[total/size][m+1];

      if (rank == 0) { // Rank 0 calcula el total
        file = fopen(dvalue, "r");
        if (file != NULL) {
          int i = 0;
          // Leemos las claves candidatas y crackeamos...
          while (fscanf(file, "%s\n", candidatas_dict[i]) == 1) {
            i++;
            fclose(file);
          }
        }

        MPI_Scatter(&candidatas_dict, (total * (m+1))/size, MPI_CHAR, &mis_candidatas_dict, (total * (m+1))/size, MPI_CHAR, 0, MPI_COMM_WORLD);

        for (int i = 0; found && i < total/size; i++) {
          if (!found) {
            compute_hash(mis_candidatas_dict[i], hashString);

            if (strcmp(hashString, secreto) == 0) {
              printf("PASSWORD FOUND!\n");
              printf("md5(%s) = %s\n", mis_candidatas_dict[i], secreto);
              found = 1;
            }
          }
          if (i % 1000 == 0) { // cADA 1000 vueltas miramos a ver si alguien lo ha encontrado
            MPI_Allreduce(&found, &newFound, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            found = newFound;
          }
        }

        if (!found) {
          printf("Password not found in file %s!\n", dvalue);
        } else {
        printf("Error: unable to open dict file!\n");
        return 1;
      }
    }

    // Si todavía no hemos encontrado la clave, ataque por fuerza bruta...
    if (!found) {
      char candidatas[CHUNK][m+1];
      char mis_candidatas[CHUNK/size][m+1];

      sprintf(candidata, "%c", avalue[0]); // -> Inicializar mejor, teniendo en cuenta n
      
      while(strlen(candidata) <= m && !found) {
        if (rank == 0) {
          //Generar claves candidatas del chunk
          for (int i = 0; i < CHUNK; i++) {
            sprintf(candidatas[i], "%s", candidata);
            next(candidata, ALPHA);
          }
        }
        MPI_Scatter(&candidatas, (CHUNK * (m+1))/size, MPI_CHAR, &mis_candidatas, (CHUNK * (m+1))/size, MPI_CHAR, 0, MPI_COMM_WORLD);

        for (int i = 0; i < CHUNK/size; i++) {
          if (!found) {
        
            compute_hash(mis_candidatas[i], hashString);
            if (strcmp(hashString, secreto) == 0) {
                printf("PASSWORD FOUND!\n");
                printf("md5(%s) = %s\n", candidatas[i], secreto);
                found = 1;
            }
          }
          if (i % 1000 == 0) { // cADA 1000 vueltas miramos a ver si alguien lo ha encontrado
            MPI_Allreduce(&found, &newFound, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            found = newFound;
            }
          }
        }
        if (!found) {
          printf("PASSWORD NOT FOUND :c\n");
      }
    } else {
    printf("Error: hash not found!\n");
    return 1;
  }
  MPI_Finalize();
  return 0;
    
  } } } 