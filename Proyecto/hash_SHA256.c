// compile: gcc hash.c -o hasher -lcrypto
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

#define SIZE 32
//El valor de salt funciona como KEY para crackear el hash
#define SALT 0

int compute_hash(char *str, unsigned char mdString[SIZE * 2 + 1])
{
  unsigned char digest[SIZE];
  SHA256_CTX ctx;
  int i;

  SHA256_Init(&ctx);
  SHA256_Update(&ctx, str, strlen(str));
  SHA256_Final(digest, &ctx);

  for (i = 0; i < SIZE; i++)
    sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i] + SALT); // sprintf imprime dentro del script

  return 0;
}

int main(int argc, char *argv[])
{
  char hashString[SIZE * 2 + 1];

  if (argc == 2)
  {
    compute_hash(argv[1], hashString);
    printf("SHA256 hash: %s\n", hashString);
  }
  else
  {
    printf("Error, usage: %s <string>\n", argv[0]);
    return -1;
  }

  return 0;
}