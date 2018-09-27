#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "compartimento_hash.h"

//função h(x)
int funcHash(Cliente* clt){
    return clt->codigo % 7;
}
//cria e seta uma hash nova para VAZIA (preenchida por -1)
FILE* setHash(){

    FILE* hash = fopen("tabHash.dat", "w+b");
    int aux = -1;
    int i;

    for (i = 0; i < 7; ++i)
    {
    	fwrite(&aux, sizeof(int), 1, hash);
    }

    rewind(hash);
    return hash;
}

//imprime uma tabela hash
void imprimeHash(){

    FILE* hash = fopen("tabHash.dat", "r+b");
    int aux;

    rewind(hash);
    printf("Tabela Hash:\n");
    while(fread(&aux, sizeof(int), 1, hash) > 0){

        printf("%d\n", aux);
    }

    printf("\n");

    rewind(hash);
}