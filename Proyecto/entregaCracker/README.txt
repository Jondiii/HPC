El programa se compila de la siguiente manera:

gcc crackerParalelo.c -o crackerParalelo -lcrypto

Esto creará un ejecutable, que se puede utilizar con el comando:

./crackerParalelo [hash a crackear]

Esto iniciará un proceso de crackeo en el que se crearán a fuerza bruta todas las combinaciones posibles del alfabeto dado.
Se puede especificar el alfabeto con el parámetro -a. Con -d se leerán las palabras de un diccionario y se usarán
para tratar de crackear el hash antes de comenzar con la fuerza bruta. Si fracasa, pasará a usar la fuerza bruta.

Este cracker genera las claves a hashear de forma paralela, por lo que es debería ser más rápido que la versión dada en
clase, que solo paralelizaba el crackeo.
