#ifndef empregado_H
#define empregado_H

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
void encadeamentoExterior(FILE* clientes, FILE *hash);
void setaProximo(int indice, int posicao, FILE* clientes);
int removeCliente(int codigo);
int insereCliente(Cliente* clt);
void criaTeste(FILE* clientes);
void desfazLigacoes(FILE* clientes);
void percorreLista(int indiceHash, int indiceCliente, FILE* clientes);

#endif /* Cliente_H */