// compile: gcc hash.c -o hasher -lcrypto
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

#define SIZE 16

// Hay que tener instalado para que funcione: sudo apt install libssl-dev

// Le pasas un str y te devuelve un string del hash SHA256
int compute_hash(char *str, unsigned char mdString[33]) {
  unsigned char digest[SIZE];
  MD5_CTX ctx;
  int i;

  MD5_Init(&ctx);
  MD5_Update(&ctx, str, strlen(str));
  MD5_Final(digest, &ctx);

  for (i = 0; i < SIZE; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);//sprintf imprime dentro del script

  return 0;
}

int main(int argc, char *argv[]) {
  char hashString[SIZE*2+1];

  if (argc == 2) { 
    compute_hash(argv[1], hashString);
    printf("MD5 hash: %s\n", hashString);
  } else {
    printf("Error, usage: %s <string>\n", argv[0]);
    return -1;
  }

  return 0;
}