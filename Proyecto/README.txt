El programa se compila de la siguiente manera:

gcc hash-entrega.c -o hash-entrega -lcrypto

Esto creará un ejecutable, que se puede utilizar con el comando:

./hash-entrega [palabra a hashear]

Y esto devolverá una palabra hasheada con el algortmo SHA256. Ejemplo:

./hasher_entrega hola
SHA256 hash: b221d9dbb083a7f33428d7c2a3c3198ae925614d70210e28716ccaa7cd4ddb79
