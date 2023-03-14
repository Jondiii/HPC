#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <omp.h>



// Diccionario en español: cp /usr/share/dict/spanish dict.txt
#define SIZE 32
// Longitud máxima de la cadena de caracteres a generar
#define MAX_LEN 4

// Caracteres alfanuméricos permitidos
const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

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

// Función para generar todas las combinaciones posibles de caracteres
void generate_strings(int len, char* str, int index) {
    int i;
    if (index == len) {
        // Hash de la cadena generada
        char hashString[SIZE*2+1];
        compute_hash(str, hashString);
        //SHA256((const unsigned char*)str, len, hash);
        printf("Hash de '%s': ", str);
        for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%02x\n", hashString[i]);
        }
        return;
    }
    for(i = 0; i < strlen(alphanum); i++) {
        str[index] = alphanum[i];
        generate_strings(len, str, index+1);
    }
}

int main() {
    int len;
    char str[MAX_LEN+1];
    int i, j;

    // Inicialización de OpenMP
    //omp_set_num_threads(omp_get_num_procs());

    // Generación de todas las combinaciones de caracteres alfanuméricos
    for(len = 1; len <= MAX_LEN; len++) {
        #pragma omp parallel for private(i, j, str)
        for(i = 0; i < strlen(alphanum); i++) {
            str[0] = alphanum[i];
            generate_strings(len, str, 1);
        }
    }

    return 0;
}