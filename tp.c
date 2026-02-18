#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "filaprocessos.h"

int main(int argc, char **argv)
{

    Pilha *p = criarPilha(); // cria a pilha de escopos

    executar(p, argc, argv); // executa o interpretador passando os nomes de arquivos

    destroiPilha(p); // limpa a memoria alocada

    return 0;
}