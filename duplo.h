#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constante_tamanho.h"

//Estrutura a ser armaenada em cada posicao do arquivo
struct usuariod{
  int chave;
  char nome[20];
  int idade;
};

int chave_teste_duplo(int chave,FILE *arq);
int hash_calculo_duplo(int chave,int indice);
void escreve_arquivo_duplo(FILE *arq);
void consulta_arquivo_duplo(FILE *arq);
void remove_arquivo_duplo(FILE *arq);
int media_acesso_duplo(FILE *arq);
struct usuariod inicializa_duplo(FILE *arq);
