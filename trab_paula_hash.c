/*Primeiro trabalho de MATA54 - Estrutura e Algorítimos de Dados II
Aluna: Paula Tavares
Trabalho: Resolução de Hashing com Encadeamento e Hashing Duplo
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "encadeamento.h"
#include "duplo.h"

const TAMANHO_ARQUIVO = 11;

int main(){

  char op,opHash;

  //Inicio das opcoes
  scanf("%c",&opHash);
  if(opHash =='l'){
    //Realiza a abertura do aquivo, caso ainda não exista cria
    FILE *arq;
    arq = fopen("arquivo.dat", "r+b");

    if(arq==NULL){
      arq = fopen("arquivo.dat", "w+b");
      if(arq==NULL){
        printf("FILE ERROR");
        return 0;
      }
      inicializa_encadeado(arq);
      fclose(arq);
    }

    scanf("%c",&op);
    while(op != 'e'){
      switch(op){
        case 'i':
        //Com a opcao 'i' faz a insercao
          escreve_arquivo(arq);
        break;
        case 'p':
        //Imprime todo o arquivo
          imprime_encadeado(arq);
        break;
        case 'c':
          consulta_arquivo(arq);
        break;
        case 'r':
          remove_arquivo(arq);
        break;
        case 'm':
          media_acesso(arq);
        break;
      }
      scanf("%c",&op);
    }
  }
  if(opHash=='d'){

    //Realiza a abertura do aquivo, caso ainda não exista cria
    FILE *arq;
    arq = fopen("arquivo.dat", "r+b");

    if(arq==NULL){
      arq = fopen("arquivo.dat", "w+b");
      if(arq==NULL){
        printf("FILE ERROR");
        return 0;
      }
      inicializa_duplo(arq);
      fclose(arq);
    }
    scanf("%c",&op);
    while(op != 'e'){
      switch(op){
        case 'i':
        //Com a opcao 'i' faz a insercao
          escreve_arquivo_duplo(arq);
        break;
        case 'p':
        //Imprime todo o arquivo
          imprime_encadeado_duplo(arq);
        break;
        case 'c':
          consulta_arquivo_duplo(arq);
        break;
        case 'r':
          remove_arquivo_duplo(arq);
        break;
        case 'm':
          media_acesso_duplo(arq);
        break;
      }
      scanf("%c",&op);
    }
  }
  return 0;
}
