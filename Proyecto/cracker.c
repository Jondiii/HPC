#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <omp.h>

// Diccionario en español: cp /usr/share/dict/spanish dict.txt

// Longitud máxima de la cadena de caracteres a generar
#define MAX_LEN 4

// Caracteres alfanuméricos permitidos
const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Función para generar todas las combinaciones posibles de caracteres
void generate_strings(int len, char* str, int index) {
    int i;
    if (index == len) {
        // Hash de la cadena generada
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((const unsigned char*)str, len, hash);
        printf("Hash de '%s': ", str);
        for(i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            printf("%02x", hash[i]);
        }
        printf("\n");
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
    omp_set_num_threads(omp_get_num_procs());

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