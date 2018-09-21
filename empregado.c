#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "empregado.h"

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
    if (0 >= fread(&lido->codigo, sizeof(int), 1, in))
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
    rewind(arq);
    int retorno = -1;
    Cliente* aux;

    while(!feof(arq))
    {
        aux = leCliente(arq);
        retorno++;
    }

    return retorno;
}

//função h(x)
int funcHash(Cliente* clt){
    return clt->codigo % 7;
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
        if(aux->liberado != 1){
            printf("Cliente %d:\n", i);
            imprimeCliente(aux);
        }

    }
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

//função para fazer a ligação de um cliente novo com a tabela hash DEU MERDA
void encadeamentoExterior(FILE* clientes, FILE *hash){

    rewind(clientes);
    rewind(hash);
    desfazLigacoes(clientes);
    setHash();

    Cliente* clt; //cliente auxiliar que usarei para ler todos os clientes da tabela CLIENTES
    int numChave; //em qual posição da hash o cliente estará alocado
    int indiceCliente; //índice do cliente para ser escrito na hash
    int aux; //usado para saber se a posição da hash está vazia ou não (-1 = vazia; qualquer outro valor = ocupado)
    int n = contaRegistros(clientes); //quantidade de elementos CLIENTES
    int i;
    rewind(clientes);

    for(i = 0; i < n; i++){ //enquanto existir clientes, o for acontece, sendo i o índice do cliente

        clt = leCliente(clientes); //leio o cliente i
        numChave = funcHash(clt); //pego o código do cliente e acho a chave dele (onde por na HASH)
        indiceCliente = i; //salvo o índice dele para escrever na HASH
        fseek(hash, numChave * sizeof(int), SEEK_SET); //procura na hash a posição após aplicar a função
        fread(&aux, sizeof(int), 1, hash); //lê o que está na posição

        if(aux == -1 && clt->liberado == 0){ //se estiver vazio

            fseek(hash, numChave * sizeof(int), SEEK_SET); //eu coloco a posição de novo onde deve
            fwrite(&indiceCliente, sizeof(int), 1, hash); //se o lugar estiver vazio, escreve o código na tabela hash
            rewind(hash);
        }

        else if(aux >= 0 && clt->liberado == 0){ //se já tiver um código na posição da tabela hash

            long unsigned int salvaPonteiro = ftell(clientes);//guardo o ponteiro para o próximo cliente (pois o altero na função setaProximo)
            setaProximo(indiceCliente, aux, clientes); //eu coloco o cliente no fim da lista após o aux, sendo indiceCliente o cliente na tabela CLIENTES
            fseek(clientes, salvaPonteiro, SEEK_SET); //volto o ponteiro pra posição que deveria estar
        }
    }

    rewind(clientes);
    rewind(hash);

    imprimeArquivo(clientes);
    imprimeHash();
}

void setaProximo(int indice, int posicao, FILE* clientes){  //indice: posicao do atual; posicao: posicao do futuro anterior

    fseek(clientes, (sizeof(Cliente) * posicao), SEEK_SET); //procuro o cliente na tabela CLIENTES (seto o ponteiro pra onde está o cliente)
    Cliente* kakyoin = leCliente(clientes); //utilizo um auxiliar para sobrescrever na tabela CLIENTES para colocar na lista

    if(kakyoin->prox == -1){ //se o cliente lido já for o último da lista

        kakyoin->prox = indice; //atualizo o último
        fseek(clientes, (sizeof(Cliente) * posicao), SEEK_SET); //vou pra onde está o último
        fwrite(&kakyoin->codigo, sizeof(int), 1, clientes); //reescrevo código
        fwrite(kakyoin->nome, sizeof(char), sizeof(kakyoin->nome), clientes); //reescrevo nome
        fwrite(&kakyoin->liberado, sizeof(int), 1, clientes); //reescrevo status
        fwrite(&kakyoin->prox, sizeof(int), 1, clientes); //reescrevo o ponteiro (ÚNICO CAMPO QUE É REALMENTE ALTERADO)
    }

    else if(kakyoin->prox >= 0){ //se o cliente lido NÃO for o último da lista

        setaProximo(indice, kakyoin->prox, clientes); //chamo a função recursivamente pro último valor da lista
    }

    else{

        printf("Erro: Número negativo diferente de -1 (tabela não preenchida corretamente).\n");
        exit(1);
    }

}

//função para remover um cliente existente
int removeCliente(int codigo){

	FILE* clientes = fopen("clientes.dat", "r+b");
    FILE* hash = fopen("tabHash.dat", "r+b");
	Cliente* auxiliar;
	int tamanho = contaRegistros(clientes);
	int aux;
    int sub = -1;
    int i, j;
    int proximo;

	rewind(clientes);

	for(i = 0; i < tamanho; i++){ //percorro os clientes

        auxiliar = leCliente(clientes); //leio o cliente

		if(auxiliar->codigo == codigo && auxiliar->liberado == 0){ //se o cliente é o que eu procuro

			auxiliar->liberado = 1;
			proximo = auxiliar->prox;
			fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
			fwrite(auxiliar, sizeof(Cliente), 1, clientes);
			printf("Cliente removido!\n\n");
            rewind(clientes);



           	for(j = 0; j < 7; j++){
           		fread(&aux, sizeof(int), 1, hash);

           		if(aux == i){
           			fseek(hash, sizeof(int) * j, SEEK_SET);
           			fwrite(&sub, sizeof(int), 1, hash);
           		}
           	}

           	Cliente* koichi;

           	for(j = 0; j < tamanho; j++){

           		koichi = leCliente(clientes);

           		if(koichi->prox == i){

           			koichi->prox = proximo;
           			fseek(clientes, sizeof(Cliente) * j, SEEK_SET);
           			fwrite(koichi, sizeof(Cliente), 1, clientes);
           		}
           	}


			return 1;
		}
	}

    printf("Cliente a remover inexistente!\n");
	return 0;
}

//função para inserir um cliente novo (opção 2, sempre checando buracos na tabela)
int insereCliente(Cliente* clt){

	FILE* clientes = fopen("clientes.dat", "r+b");
	FILE* hash = fopen("tabHash.dat", "r+b");

	Cliente* aux;
	int tamanho = contaRegistros(clientes);
	int i;
	rewind(clientes);

	for(i = 0; i < tamanho; i++){

		aux = leCliente(clientes); //leio os clientes

		if(aux->codigo == clt->codigo && aux->liberado == 0){ //se o cliente auxiliar tiver o mesmo código do cliente que quero inserir

			printf("Cliente já existente!\n");
			return 0; //retorno 0, pois o cliente já existe e não foi gravado
		}

		else if(aux->liberado == 1){ //se o espaço que eu li estiver liberado

			fseek(clientes, i * sizeof(Cliente), SEEK_SET); //eu seto o ponteiro de volta
			fwrite(clt, sizeof(Cliente), 1, clientes); //reescrevo as informações com o cliente novo que inserirei
			printf("Cliente inserido no meio da lista! Será necessário refazer o Encadeamento Exterior para atualizar sua lista.\n\n");
			return 1;
		}
	}

	fwrite(clt, sizeof(Cliente), 1, clientes);
	printf("\nCliente inserido no fim da lista! Será necessário refazer o Encadeamento Exterior para atualizar sua lista.\n\n");

	return 2;
}

//função pra criar a hash e o arquivo padrão do exemplo do ubiratam
void criaTeste(FILE* clientes){

    Cliente* c1 = criaCliente(49, "João");
    Cliente* c2 = criaCliente(59, "Maria");
    Cliente* c3 = criaCliente(103, "Ana");
    Cliente* c4 = criaCliente(3, "Jose");
    Cliente* c5 = criaCliente(51, "Carla");
    Cliente* c6 = criaCliente(87, "Bia");

    escreveCliente(clientes, c1);
    escreveCliente(clientes, c2);
    escreveCliente(clientes, c3);
    escreveCliente(clientes, c4);
    escreveCliente(clientes, c5);
    escreveCliente(clientes, c6);

    setHash();
}

void desfazLigacoes(FILE* clientes){

    Cliente* iggy;
    int n = contaRegistros(clientes);
    int i;
    int nulo = -1;
    rewind(clientes);

    for(i = 0; i < n; i++){

        iggy = leCliente(clientes);
        fseek(clientes, sizeof(Cliente) * i, SEEK_SET);
        iggy->prox = nulo;
        fwrite(&iggy->codigo, sizeof(int), 1, clientes); //reescrevo código
        fwrite(iggy->nome, sizeof(char), sizeof(iggy->nome), clientes); //reescrevo nome
        fwrite(&iggy->liberado, sizeof(int), 1, clientes); //reescrevo status
        fwrite(&iggy->prox, sizeof(int), 1, clientes); //reescrevo o ponteiro (ÚNICO CAMPO QUE É REALMENTE ALTERADO)
    }

    rewind(clientes);
}
