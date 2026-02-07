#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "filaprocessos.h"

int main(int argc, char **argv){

    Pilha *p = criarPilha();

    executar(p, argc, argv);

    destroiPilha(p);

    return 0;
}