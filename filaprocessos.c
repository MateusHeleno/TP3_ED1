#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filaprocessos.h"



Pilha *criarPilha(){
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    
    if(!p)
        return NULL;
    
    p->topo = NULL;
    p->tam = 0;

    return p;
}

Pilha *destroiPilha(Pilha *p){
    if(!p)
        return NULL;
    
    while(p->topo != NULL){
        desempilha(p);
    }

    free(p);
    return NULL;
}

void empilha(Pilha *p,Escopo *e){
    if(!p)
        return;
    if(!e)
        return;

    e->prox = p->topo->prox;
    
    p->topo = e;

    p->tam++;
}

void desempilha(Pilha *p){
    if(!p)
        return;
    if(p->tam == 0)
        return;

    Escopo *aux = p->topo;

    p->topo = p->topo->prox;

    p->tam--;
    destroiEscopo(aux);// la dentro ela destroi a arvore;
    free(aux);
}

void executar(){

}

Escopo *criarEscopo(){
    Escopo *e = (Escopo*)malloc(sizeof(Escopo));
    if(!e)
        return NULL;

    e->raiz = NULL;
    e->prox = NULL;
    e->tamAr = 0;

    return e;
}

Escopo *destroiEscopo(Escopo *e){
    if(!e)
        return NULL;

    destroiAr(e->raiz);

    e->raiz = NULL;
    e->tamAr=0;

    free(e);
    return NULL;

}

void adicionaVar(){

}

No *criaNo(){
    return NULL;
}

void destroiAr(No *no){
    if(!no)
        return;

    destroiAr(no->esq);
    destroiAr(no->dir);

    free(no);
}





