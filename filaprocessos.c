#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "filaprocessos.h"

Pilha *criarPilha()
{
    Pilha *p = (Pilha *)malloc(sizeof(Pilha));

    if (!p)
        return NULL;

    p->topo = NULL;
    p->tam = 0;

    return p;
}

Pilha *destroiPilha(Pilha *p)
{
    if (!p)
        return NULL;

    while (p->topo != NULL) // esvazia a pilha antes de liberar a estrutura
    {
        desempilha(p);
    }

    free(p);
    return NULL;
}

void adicionaPilha(Pilha *p, Escopo *e)
{
    if (!p || !e)
        return;

    e->prox = p->topo; // o prox do novo e o antigo topo
    p->topo = e;       // atualiza topo
    p->tam++;
}

void desempilha(Pilha *p)
{
    if (!p || p->tam == 0)
        return;

    Escopo *aux = p->topo;
    p->topo = p->topo->prox; // topo desce pro escopo de baixo

    p->tam--;
    destroiEscopo(aux); // libera o no da pilha e da arvore
}

void executar(Pilha *p, int argc, char **argv)
{
    FILE *arq = abreArquivo(argc, argv);
    if (!arq)
        return;

    char palavra[TAM_PALA];

    while (fscanf(arq, "%s", palavra) != EOF) // leitura linha a linha
    {

        int comando = analiseArq(palavra);

        switch (comando)
        {
        case 1: // begin
        {
            Escopo *e = criarEscopo();
            adicionaPilha(p, e);
            break;
        }

        case 2: // var
        {
            char valor[TAM_PALA], var[TAM_PALA], lixo[TAM_PALA];

            if (p->topo == NULL) // verifica se tem espaco
            {
                printf("Escopo nao aberto\n");
                destroiPilha(p);
                fclose(arq);
                exit(1);
            }

            fscanf(arq, "%s %s %s", var, lixo, valor); // "var x = 10"
            adicionaVar(p, var, valor);
            break;
        }

        case 3: // print
        {
            char varBusca[TAM_PALA];
            if (p->topo == 0)
            {
                printf("Escopo nao aberto\n");
                destroiPilha(p);
                fclose(arq);
                exit(1);
            }

            fscanf(arq, "%s", varBusca);
            char *resultado = buscarVar(p, varBusca);

            if (resultado == NULL)
            {
                printf("Variavel %s nao declarada\n", varBusca);
                destroiPilha(p);
                fclose(arq); // evita o vazamento de memoria
                exit(1);
            }
            else
            {
                printf("%s\n", resultado);
            }
            break;
        }
        case 4: // end
        {
            if (p->topo == NULL)
            {
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
    if (p->tam > 0) // se sobrar escopo na pilha
    {
        printf("Escopo não fechado\n");
        fclose(arq);
        destroiPilha(p);
        exit(1);
    }

    fclose(arq);
}

Escopo *criarEscopo()
{
    Escopo *e = (Escopo *)malloc(sizeof(Escopo));
    if (!e)
        return NULL;

    e->raiz = NULL;
    e->prox = NULL;
    e->tamAr = 0;

    return e;
}

Escopo *destroiEscopo(Escopo *e)
{
    if (!e)
        return NULL;

    destroiAr(e->raiz); // destroi recursivamente a arvore do escopo

    e->raiz = NULL;
    e->tamAr = 0;

    free(e);
    return NULL;
}

void adicionaVar(Pilha *p, char *var, char *valor) // adiciona variavel na arvore do escopo atual
{

    if (p == NULL)
        return;

    if (adicionaNo(&(p->topo->raiz), var, valor)) // se retorna true eh uma chave nova se retorna false eh uma chave ja existente
        p->topo->tamAr++;
}

char *buscarVar(Pilha *p, char *varBusca) // busca a chave nas pilhas
{
    Escopo *atual = p->topo;

    while (atual != NULL) // passa pelos escopos do topo ate a base
    {
        No *noEncontrado = buscarAr(atual->raiz, varBusca);
        if (noEncontrado != NULL)
        {
            return noEncontrado->valor; // escopo mais recente encontrado
        }
        atual = atual->prox; // desce pro escopo anterior
    }
    return NULL; // retorna NULL sinalizando erro, tratamento na funcao executar
}

void imprimirVar(Pilha *p, char *varBusca) // funcao de impressao
{
    char resultado[TAM_PALA];
    strcpy(resultado, buscarVar(p, varBusca));
    printf("%s\n", resultado);
}

No *buscarAr(No *raiz, char *varBusca) // busca recursiva na arvore binaria
{
    if (raiz == NULL)
        return NULL;

    int comp = strcmp(varBusca, raiz->var);

    if (comp < 0)
        return buscarAr(raiz->esq, varBusca); // procura na esquerda
    else if (comp > 0)
        return buscarAr(raiz->dir, varBusca); // procura na direira
    else
        return raiz; // encontrou
}

bool adicionaNo(No **raiz, char *var, char *valor) // insere na arvore, retorna true se criar e false se nao
{
    if (*raiz == NULL)
    {
        *raiz = criaNo(var, valor);
        return true;
    }

    int comp = strcmp(var, (*raiz)->var);

    if (comp < 0)
        return adicionaNo((&(*raiz)->esq), var, valor);
    else if (comp > 0)
        return adicionaNo((&(*raiz)->dir), var, valor);
    else
    {
        strcpy((*raiz)->valor, valor);
        return false;
    }

    return true;
}

No *criaNo(char *var, char *valor)
{
    No *novo = (No *)malloc(sizeof(No));
    if (!novo)
        return NULL;

    strcpy(novo->var, var);
    strcpy(novo->valor, valor);
    novo->esq = novo->dir = NULL;
    return novo;
}

void destroiAr(No *no)
{
    if (!no)
        return;

    destroiAr(no->esq);
    destroiAr(no->dir);

    free(no);
}

FILE *abreArquivo(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s nome_do_arquivo\n", argv[0]);
        return NULL;
    }

    FILE *arq = fopen(argv[1], "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    return arq;
}

int analiseArq(char *palavra)
{
    if (!strcmp(palavra, "begin"))
        return 1;
    if (!strcmp(palavra, "var"))
        return 2;
    if (!strcmp(palavra, "print"))
        return 3;
    if (!strcmp(palavra, "end"))
        return 4;
    return 5; // erro
}
