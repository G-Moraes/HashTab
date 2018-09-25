#include <stdio.h>
#include <stdlib.h>
#include "empregado.h"


int main ()
{
	int opcao;

	printf("Bem-vindo ao trabalho. O que deseja?\n\n");	
	printf("0: Criar arquivos vazios\n1: Criar arquivo base clientes e hash\n2: imprimir arquivo clientes\n3: imprimir tabela hash\n");
	printf("4: Inserir cliente\n5: Remover clientes\n6: Desfazer ligações dos clientes e refazer tabela hash\n7: Aplicar o encadeamento exterior\n\n");
	scanf("%d", &opcao);
	printf("\n");
	
	if(opcao == 0){
		
		FILE* clientes = fopen("clientes.dat", "w+b");
    	setHash();
	}

	else if(opcao == 1){

		FILE* clientes = fopen("clientes.dat", "w+b");
		criaTeste(clientes);
		printf("Arquivo criado com sucesso.\n");
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

		printf("Digite código e nome do cliente:\n");
		scanf("%d %[^\n]", &codigo, nome);
		usuario = criaCliente(codigo, nome);
		insereCliente(usuario);
	}

	else if(opcao == 5){

		int codigo;

		printf("Digite o código do cliente a ser deletado:\n");
		scanf("%d", &codigo);
		removeCliente(codigo);
	}

	else if(opcao == 6){

		FILE* clientes = fopen("clientes.dat", "r+b");

		//desfazLigacoes(clientes);
		setHash();

		printf("Ligações desfeitas e tabela hash refeita.\n");
	}

	else if(opcao == 7){

		FILE* clientes = fopen("clientes.dat", "r+b");
   		FILE* hash = fopen("tabHash.dat", "r+b");

   		//encadeamentoExterior(clientes, hash);
	}

}

