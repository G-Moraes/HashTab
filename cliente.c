#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "compartimento_hash.h"

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

Cliente* criaCliente(int c, char* n)
{
    Cliente* new = (Cliente*) malloc(sizeof(Cliente));
    new->codigo = c;
    new->liberado = 0;
    new->prox = -1;
    strcpy(new->nome, n);

    return new;
}

void escreveCliente(FILE* out, Cliente* e)
{
    Cliente* aux;
    for(int i = 0; i <= contaRegistros(out); i++)
    {
        aux = leCliente(out);
    }
    fwrite(&e->codigo, sizeof(int), 1, out);
    fwrite(e->nome, sizeof(char), sizeof(e->nome), out);
    fwrite(&e->liberado, sizeof(int), 1, out);
    fwrite(&e->prox, sizeof(int), 1, out);
}

Cliente* leCliente(FILE* in)
{
    Cliente* lido = (Cliente*) malloc(sizeof(Cliente));
    if (0 > fread(&lido->codigo, sizeof(int), 1, in))
    {
        free(lido);
        return NULL;
    }

    fread(lido->nome, sizeof(char), sizeof(lido->nome), in);
    fread(&lido->liberado, sizeof(int), 1, in);
    fread(&lido->prox, sizeof(int), 1, in);

    return lido;
}

void imprimeCliente(Cliente* e)
{

    if(e == NULL)
    {
        printf("Cliente não encontrado!\n");
    }

    else if(e->liberado == 0)
    {
        printf("Codigo: %d\nNome: %s\nStatus: %d\nPróximo Cliente: %d\n\n", e->codigo, e->nome, e->liberado, e->prox);
    }

}

int contaRegistros(FILE* arq)
{
    //rewind(arq);
    int retorno = -1;
    Cliente* aux;

    while(!feof(arq))
    {
        aux = leCliente(arq);
        retorno++;
    }

    free(aux);
    return retorno;
}

//função para imprimir todo o arquivo
void imprimeArquivo(FILE *f){

	printf("Arquivo Clientes:\n\n");
    int n = contaRegistros(f);
    int i;
    Cliente* aux;

    rewind(f);

    for(i = 0; i < n; i++){

        aux = leCliente(f);
        
        if(aux->liberado == 0){
            printf("Cliente %d:\n", i);
            imprimeCliente(aux);
        }
    }

    if(n == 0){
        
        printf("Arquivo vazio!\n\n");
        return;
    }

    free(aux);
    fclose(f);
}

//função para remover um cliente existente REFAZER
int removeCliente(int codigo){

	FILE* clientes = fopen("clientes.dat", "r+b");
    FILE* hash = fopen("tabHash.dat", "r+b");

    Cliente* teste = criaCliente(codigo, "Santana");
    int chave = funcHash(teste);
    int leituraHash;
    int sub = -1;

    fseek(hash, sizeof(int) * chave, SEEK_SET);
    fread(&leituraHash, sizeof(int), 1, hash);

    if(leituraHash == -1){ //se a posição na hash está vazia

        printf("Cliente inexistente!\n");
        return 0;
    } 

    else{ //se tem algo na hash

        fseek(clientes, sizeof(Cliente) * leituraHash, SEEK_SET);
        teste = leCliente(clientes); //leio o cliente da hash

        if(teste->codigo == codigo && teste->prox == -1){ //se ele não tem próximo

            teste->liberado = 1;
            fseek(hash, sizeof(int) * chave, SEEK_SET);
            fwrite(&sub, sizeof(int), 1, hash);
            fseek(clientes, sizeof(Cliente) * leituraHash, SEEK_SET);
            fwrite(teste, sizeof(Cliente), 1, clientes);
            printf("Cliente excluído!\n\n");
            return 1;
        }

        else{ //se não é o elemento que eu to procurando, ele está na lista

            if(teste->codigo == codigo && teste->prox != -1){ //se for o cabeça da lista
            	
            	teste->liberado = 1;
            	sub = teste->prox;
            	fseek(hash, sizeof(int) * chave, SEEK_SET);
            	fwrite(&sub, sizeof(int), 1, hash);
            	fseek(clientes, sizeof(Cliente) * leituraHash, SEEK_SET);
            	fwrite(teste, sizeof(Cliente), 1, clientes);
            	printf("Cliente excluído!\n\n");
            	return 1;

            }

            else{
            	//talvez de merda aqui, substitua leituraHash por chave
	            percorreListaDel(teste->prox, leituraHash, codigo, clientes);
	            printf("Cliente Excluído!\n\n");
	            return 1;
            }

        }
    }
}

//função auxiliar que percorre recursivamente a lista encadeada para remoção
void percorreListaDel(int next, int atual, int codigo, FILE* clientes){

    Cliente* aux;

    fseek(clientes, sizeof(Cliente) * next, SEEK_SET);
    aux = leCliente(clientes);

    if(aux->codigo == codigo && aux->prox == -1){ //se eu cheguei no fim da lista e achei o registro a ser excluído

        aux->liberado = 1;

        Cliente* rohan;
        fseek(clientes, sizeof(Cliente) * next, SEEK_SET);
        fwrite(aux, sizeof(Cliente), 1, clientes); //removo o cliente
        fseek(clientes, sizeof(Cliente) * atual, SEEK_SET);
        rohan = leCliente(clientes);
        rohan->prox = -1;
        fseek(clientes, sizeof(Cliente) * atual, SEEK_SET);
        fwrite(rohan, sizeof(Cliente), 1, clientes);
    }

    else if(aux->codigo == codigo && aux->prox >= 0){ //se eu achei o registro no meio de uma lista encadeada

        aux->liberado = 1; //marco auxiliar como liberado
        int proximo = aux->prox;

        fseek(clientes, sizeof(Cliente) * next, SEEK_SET);
        fwrite(aux, sizeof(Cliente), 1, clientes); //apago o registro desejado

        Cliente* bowsette;
        fseek(clientes, sizeof(Cliente) * atual, SEEK_SET);
        bowsette = leCliente(clientes);
        bowsette->prox = proximo; //atualizo o ponteiro do registro que apontava pro removido, pra apontar para onde o removido apontava
        fseek(clientes, sizeof(Cliente) * atual, SEEK_SET);
        fwrite(bowsette, sizeof(Cliente), 1, clientes); //sobrescrevo
    }

    else{ //se eu não achei o registro

        int ponteiroProx = aux->prox;

        Cliente* Jonathan;
        fseek(clientes, sizeof(Cliente) * atual, SEEK_SET);
        Jonathan = leCliente(clientes);

        int JoJoProx = Jonathan->prox;

        percorreListaDel(ponteiroProx, JoJoProx, codigo, clientes);
    }
}

//função para inserir um cliente novo (opção 2, sempre checando buracos na tabela) TERMINAR
int insereCliente(Cliente* clt){

	FILE* clientes = fopen("clientes.dat", "r+b");
	FILE* hash = fopen("tabHash.dat", "r+b");

    int chave = funcHash(clt);
    int i;
    int leituraHash;
    int n = contaRegistros(clientes);
    rewind(clientes);
    Cliente* aux;

    if(n == 0){ //se não existem clientes

        fwrite(clt, sizeof(Cliente), 1, clientes);
        fseek(hash, sizeof(int) * chave, SEEK_SET);
        fwrite(&n, sizeof(int), 1, hash);
        printf("Primeiro cliente inserido na tabela!\n\nCliente 0:\n");
        imprimeCliente(clt);
        return 1;
    }

    fseek(hash, sizeof(int) * chave, SEEK_SET);
    fread(&leituraHash, sizeof(int), 1, hash);

    rewind(clientes);

    if(leituraHash == -1){ //se a posição da hash ta vazia
        
        for(i = 0; i < n; i++){

            aux = leCliente(clientes); //leio os clientes
            
            if(aux->liberado == 1){ //quando encontrar a primeira posição livre na tabela

                fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
                fwrite(clt, sizeof(Cliente), 1, clientes);
                fseek(hash, sizeof(int) * chave, SEEK_SET);
                fwrite(&i, sizeof(int), 1, hash);
                printf("Cliente inserido no meio do arquivo!\n\nCliente %d:\n", i);
                imprimeCliente(clt);
                rewind(clientes);
                rewind(hash);
                fclose(clientes);
                fclose(hash);
                return 1;
            }
        }

        fwrite(clt, sizeof(Cliente), 1, clientes); //se não encontrar um buraco na tabela, escrevo no fim
        fseek(hash, sizeof(int) * chave, SEEK_SET);
        fwrite(&i, sizeof(int), 1, hash);
        printf("Cliente inserido no fim do arquivo!\n\nCliente %d:\n", n);
        imprimeCliente(clt);
        free(aux);
        rewind(clientes);
        rewind(hash);
        fclose(clientes);
        fclose(hash);
        return 1;
    }

    else{ //se tem algo na hash

        fseek(clientes, sizeof(Cliente) * leituraHash, SEEK_SET);
        aux = leCliente(clientes); //leio o cliente da hash
        rewind(clientes);

        for(i = 0; i < n; i++){ //procuro a próxima posição vazia na tabela

            Cliente* okuyasu = leCliente(clientes);

            if(okuyasu->liberado == 1){ //se eu achei um espaço vazio na tabela

                fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
                fwrite(clt, sizeof(Cliente), 1, clientes); //escrevo o cliente que eu pretendo inserir
                printf("Cliente inserido no meio do arquivo!\n\nCliente %d:\n", i);
                imprimeCliente(clt);
                percorreListaAdd(leituraHash, i, clientes);
                free(aux);
                rewind(clientes);
                rewind(hash);
                fclose(clientes);
                fclose(hash);
                return 1;
            }

            else if(okuyasu->codigo == clt->codigo && okuyasu->liberado == 0){ //se encontrei o cliente
                
                printf("Cliente encontrado! Considere alterar o valor do código de cliente!\n");
                free(aux);
                rewind(clientes);
                rewind(hash);
                fclose(clientes);
                fclose(hash);
                return 0;
            }
        }

        fwrite(clt, sizeof(Cliente), 1, clientes);
        printf("Cliente inserido no fim do arquivo!\n\nCliente %d:\n", i);
        imprimeCliente(clt);
        percorreListaAdd(leituraHash, i, clientes);
        free(aux);
        rewind(clientes);
        rewind(hash);
        fclose(clientes);
        fclose(hash);
        return 1;
    }
}

//função auxiliar que percorre recursivamente a lista encadeada para inserção
void percorreListaAdd(int indiceHash, int indiceCliente, FILE* clientes){

    Cliente* aux;
    
    fseek(clientes, sizeof(Cliente) * indiceHash, SEEK_SET);
    aux = leCliente(clientes);
    int valor = aux->prox;

    if(valor == -1){ //se for o fim da lista
        
        aux->prox = indiceCliente;
        fseek(clientes, sizeof(Cliente) * indiceHash, SEEK_SET);
        fwrite(aux, sizeof(Cliente), 1, clientes);
    }

    else if(valor >= 0){ //se o registro está encadeado e não é o último registro
        
        percorreListaAdd(valor, indiceCliente, clientes);
    }

    free(aux);
}

int busca(int codigo){

    FILE* clientes = fopen("clientes.dat", "r+b");
    FILE* hash = fopen("tabHash.dat", "r+b");

    Cliente* wamuu = criaCliente(codigo, "Wamuu");
    int chave = funcHash(wamuu);
    int leituraHash;

    fseek(hash, sizeof(int) * chave, SEEK_SET);
    fread(&leituraHash, sizeof(int), 1, hash);

    if(leituraHash == -1){ //se a posição da hash ta vazia

        printf("Registro inexistente!\n\n");
        return 0;
    }

    else{ //se tem algo na hash

        fseek(clientes, sizeof(Cliente) * leituraHash, SEEK_SET);
        wamuu = leCliente(clientes);

        if(wamuu->codigo == codigo){ //se encontrei o registro na cabeça da lista

            printf("Registro encontrado!\n\nCliente %d:\n", leituraHash);
            imprimeCliente(wamuu);
            free(wamuu);
            fclose(clientes);
            return 1;
        }

        else{ //se não encontrei como cabeça da lista

            buscaEncadeada(chave, wamuu->prox, codigo, clientes);
            free(wamuu);
            fclose(clientes);

        }    
    }
}

void buscaEncadeada(int chave, int nextEnd, int codigo, FILE* clientes){

    if(nextEnd == -1){

        printf("Registro inexistente!\n\n");
        return;
    }

    else{ //se não chegou no fim da lista

        Cliente* keicho;
        fseek(clientes, sizeof(Cliente) * nextEnd, SEEK_SET);
        keicho = leCliente(clientes);
        
        if(keicho->codigo == codigo){

            printf("Registro encontrado!\n\nCliente %d:\n", nextEnd);
            imprimeCliente(keicho);
            free(keicho);
            return;
        }

        else{ //se não cheguei no fim da lista, e ainda não encontrei o registro buscado

            buscaEncadeada(nextEnd, keicho->prox, codigo, clientes);
        }
    }
}