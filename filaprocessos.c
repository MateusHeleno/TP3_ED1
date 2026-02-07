#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

    e->prox = p->topo;
    
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
}

void executar(Pilha *p,int argc, char **argv){
    FILE* arq = abreArquivo(argc,argv);
    if(!arq)
        return;

    char palavra[TAM_PALA];
    while(fscanf(arq,"%s",palavra) != EOF){
        
        int comando = analiseArq(palavra);

        switch(comando){
            case 1:{
                Escopo *e = criarEscopo();
                empilha(p,e);
                break;
            }

            case 2:{
                char valor[TAM_PALA], var[TAM_PALA], lixo[TAM_PALA];
                
                if(p->topo == 0){
                    printf("Escopo nao aberto\n");
                    destroiPilha(p);
                    fclose(arq);
                    exit(1);
                }

                fscanf(arq,"%s %s %s",var,lixo,valor);
                adicionaVar(p,var,valor);
                break;
            }

            case 3:{
                char varBusca[TAM_PALA];
                if(p->topo == 0){
                    printf("Escopo nao aberto\n");
                    destroiPilha(p);
                    fclose(arq);
                    exit(1);
                }
                
                fscanf(arq,"%s",varBusca);
                imprimirVar(p,varBusca);
                break;
            }
            case 4:{ 
                if(p->topo == 0){
                    printf("Escopo nao aberto\n");
                    destroiPilha(p);
                    fclose(arq);
                    exit(1);
                }
                desempilha(p);
                break;
            }
        }
    }
    if (p->tam > 0) {
        printf("Escopo não fechado\n"); 
        fclose(arq);
        destroiPilha(p);
        exit(1);
    }

    fclose(arq);
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

void adicionaVar(Pilha *p,char *var,char *valor){ // chamo com pilha para mascarar funcionamento interno, e ficar padronizado

    if (p == NULL)
        return;
    
    if (adicionaNo(&(p->topo->raiz), var, valor)) 
        p->topo->tamAr++;
    
}

char *buscarVar(Pilha *p,char* varBusca){ // voce busca entre as pilhas
    Escopo *atual = p->topo;

    
    while (atual != NULL) {
        No *noEncontrado = buscarAr(atual->raiz, varBusca);
        if (noEncontrado != NULL) {
            return noEncontrado->valor; 
        }
        atual = atual->prox; 
    }
    printf("Variavel %s nao declarada\n",varBusca);
    destroiPilha(p);
    exit(1);
    return NULL; 
}

void imprimirVar(Pilha *p,char *varBusca){
    char resultado[TAM_PALA];
    strcpy(resultado,buscarVar(p,varBusca));
    printf("%s\n",resultado);
}

No* buscarAr(No* raiz, char* varBusca){ //busca entre as arvores
    if(raiz == NULL)
        return NULL;

    int comp = strcmp(varBusca,raiz->var);

    if(comp < 0)
        return buscarAr(raiz->esq,varBusca);
    else if(comp == 0)
        return raiz;
    else 
        return buscarAr(raiz->dir,varBusca);

}

bool adicionaNo(No **raiz,char *var,char *valor){
    if(*raiz == NULL){
        *raiz = criaNo(var,valor);
        return true;
    }

    int comp = strcmp(var, (*raiz)->var);

    if(comp < 0 )
        return adicionaNo((&(*raiz)->esq),var,valor);
    else if (comp > 0)
        return adicionaNo((&(*raiz)->dir),var,valor);
    else{   
        strcpy((*raiz)->valor,valor);
        return false;
    }

    return true;
}

No *criaNo(char *var,char *valor){
    No* novo = (No*)malloc(sizeof(No));
    if(!novo)
        return NULL;

    strcpy(novo->var, var);
    strcpy(novo->valor, valor);
    novo->esq = novo->dir = NULL;
    return novo;
    
}

void destroiAr(No *no){
    if(!no)
        return;

    destroiAr(no->esq);
    destroiAr(no->dir);

    free(no);
}

FILE* abreArquivo(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s nome_do_arquivo\n", argv[0]);
        return NULL;
    }

    FILE *arq = fopen(argv[1], "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    return arq;
}

int analiseArq(char *palavra){
    int valor;

    if(!strcmp(palavra,"begin"))
        valor = 1;
    else if(!strcmp(palavra,"var"))
        valor = 2;
    else if(!strcmp(palavra,"print"))
        valor = 3;
    else if(!strcmp(palavra,"end"))
            valor = 4;
    else 
        valor = 5;

    return valor;
}

