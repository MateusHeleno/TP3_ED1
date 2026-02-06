#ifndef  FILAPROCESSOS_H
#define FILAPROCESSOS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// structs
typedef struct {
    char var[16];
    int valor;
}Escopo;

typedef struct {
    int x;
}Pilha;


// TAD PILHA

Pilha *criarPilha();
Pilha *destroiPilha();
void empilha();
void desempilha();
void executar();

// TAD ESCOPO
Escopo *criarEscopo();
Escopo *destroiEscopo();
void adicionaVar();

#endif



