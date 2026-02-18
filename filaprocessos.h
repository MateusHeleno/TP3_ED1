#ifndef FILAPROCESSOS_H
#define FILAPROCESSOS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAM_PALA 16

// structs

typedef struct No // No da arvore
{
    char var[TAM_PALA];   // nome da variavel
    char valor[TAM_PALA]; // valor de variavel
    struct No *esq;
    struct No *dir;
} No;

typedef struct Escopo // No da pilha
{
    No *raiz;            // raiz da arvore
    struct Escopo *prox; // ponteiro pro escopo de baixo
    int tamAr;           // tamanho da arvore
} Escopo;

typedef struct // struct da pilha
{
    Escopo *topo; // ponteiro pro topo da pilha
    int tam;      // quantidade de escopos da pilha
} Pilha;

// TAD PILHA
Pilha *criarPilha();
Pilha *destroiPilha(Pilha *p);
void adicionaPilha(Pilha *p, Escopo *e);
void desempilha(Pilha *p);
void executar(Pilha *p, int argc, char **argv);

// TAD ESCOPO
Escopo *criarEscopo();
Escopo *destroiEscopo(Escopo *e);
void adicionaVar(Pilha *p, char *var, char *valor);
char *buscarVar(Pilha *p, char *varBusca);
void imprimirVar(Pilha *p, char *varBusca);
No *buscarAr(No *raiz, char *varBusca);

// TAD NO
bool adicionaNo(No **raiz, char *var, char *valor);
No *criaNo(char *var, char *valor);
void destroiAr(No *no);

// aux
FILE *abreArquivo(int argc, char *argv[]);
int analiseArq(char *palavra);

#endif
