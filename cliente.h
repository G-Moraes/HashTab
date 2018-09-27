#ifndef Cliente_H
#define Cliente_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente 
{
    int codigo;
    char nome[100];
    int liberado; 
    int prox; 

} Cliente; 

int cmpfunc (const void * a, const void * b);
Cliente* criaCliente(int c, char* n);
void escreveCliente(FILE* out, Cliente* e);
Cliente* leCliente(FILE* in);
void imprimeCliente(Cliente* e);
int contaRegistros(FILE* arq);
int funcHash(Cliente* clt);
void imprimeArquivo(FILE *f);
FILE* setHash();
void imprimeHash();
int removeCliente(int codigo);
void percorreListaDel(int next, int atual, int codigo, FILE* clientes);
int insereCliente(Cliente* clt);
void percorreListaAdd(int indiceHash, int indiceCliente, FILE* clientes);
void desfazLigacoes(FILE* clientes);
int busca(int codigo);
void buscaEncadeada(int chave, int nextEnd, int codigo, FILE* clientes);


#endif /* Cliente_H */
