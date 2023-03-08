#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char secreto[32+1];
    //copiar el hash desde argv[1]
    //bucle (while)
    //  100000 x next(candidata) -> array
    //  #pragma omp parallel for
    //  for (int i = 0; i < 10000; i++) {
    //   recorrer el array con 10000 claves candidatas
    //   hilo0 -> 0 - 2499; hilo1 -> 2500 - 4999; hilo2 -> 5000 - 7499; hilo3 -> 7500 - 9999
    //   if compute_hash(candidata) == secreto {
    //    print("Encontrado\n"); break;
    //   }
    //  }
    //  generar siguiente clave candidata
    //  calcular hash de la clave candidata
    //  comprobar si hash == secreto

    sprintf(candidata, "%c", avalue[0]); // Inicializar mejor teniendo en cuenta el valor del tamaño mínimo de la clave
    if (!found) {
        candidata = next(candidata, alfabeto);
        while(strlen(candidata) <= maxLen) {
            compute_hash(candidata, hashString);
            if (strcmp(hashString, secreto) == 0) {
                found = 1;
            }
            candidata = next(candidata, avalue); // Programar next() -> cómo calcular la siguiente clave teniendo en cuenta la que tengo ahora en función del abecedario
        }
    }
}