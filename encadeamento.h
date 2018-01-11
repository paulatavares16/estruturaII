#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constante_tamanho.h"

//Estrutura a ser armaenada em cada posicao do arquivo
struct usuario{
  int chave;
  char nome[20];
  int idade;
  int pointer;
};

int chave_teste(int chave, FILE *arq);
int hash_calculo(int chave);
void escreve_arquivo(FILE *arq);
void consulta_arquivo(FILE *arq);
void remove_arquivo(FILE *arq);
int media_acesso(FILE *arq);
struct usuario inicializa_encadeado(FILE *arq);
