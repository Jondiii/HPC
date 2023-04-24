for NPROC in 1 2 3 4 5 6 7 8
do
    for SECRETO in abc hola melon sandia
    do
        for ALPHA in abcdefghijkl 0123456789abcd 0123456789abcABC
        do
            echo "Probando con $NPROC proceso, alfabeto $ALPHA y secreto: $SECRETO..."
            #export OMP_NUM_THREADS=$NPROC
            time mpirun -np $NPROC ./crackerPabloMPI $(./hasher $SECRETO) -a $ALPHA
            #time ./crackerPablo $(./hasher $SECRETO) -a $ALPHA
        done
    done
done > pruebasRendimiento.txt #Columna: numero de procesadores. Fila: caso (alfabeto + palabra a crackear)
# Speedup: dividir entre lo que tardó con 1 solo procesador