#include <stdio.h>
#include <stdlib.h>
#include "empregado.h"


int main ()
{
	int opcao;

	printf("Bem-vindo ao trabalho. O que deseja?\n\n");	
	printf("1: Criar arquivos vazios\n2: imprimir arquivo cliente\n3: imprimir tabela hash\n");
	printf("4: Inserir cliente\n5: Remover cliente\n6: Buscar cliente\n\n");
	scanf("%d", &opcao);
	printf("\n");
	
	if(opcao == 1){
		
		FILE* clientes = fopen("clientes.dat", "w+b");
    	setHash();
    	printf("Arquivos zerados!\n\n");
	}

	else if(opcao == 2){
		
		FILE* clientes = fopen("clientes.dat", "r+b");
		imprimeArquivo(clientes);
	}

	else if(opcao == 3){

		imprimeHash();
	}

	else if(opcao == 4){

		Cliente* usuario;
		int codigo;
		char nome[100];

		printf("Digite código e nome do cliente:\n\n");
		scanf("%d %[^\n]", &codigo, nome);
		printf("\n");
		usuario = criaCliente(codigo, nome);
		insereCliente(usuario);
	}

	else if(opcao == 5){

		int codigo;

		printf("Digite o código do cliente a ser deletado:\n\n");
		scanf("%d", &codigo);
		printf("\n");
		removeCliente(codigo);
	}

	else if(opcao == 6){

		int codigo;

		printf("Digite o código do cliente a ser buscado:\n\n");
		scanf("%d", &codigo);
		printf("\n");
		busca(codigo);
	}
}

