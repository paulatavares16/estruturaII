/*Primeiro trabalho de MATA54 - Estrutura e Algorítimos de Dados II
Aluna: Paula Tavares
Trabalho: Resolução de Hashing com Encadeamento e Hashing Duplo
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "encadeamento.h"
// #include "duplo.h"

// Tamanho do arquivo como variável global
int TAMANHO_ARQUIVO=11;

//Estrutura a ser armaenada em cada posicao do arquivo
struct usuario{
  int chave;
  char nome[20];
  int idade;
  int pointer;
};

int chave_teste_duplo(int chave,FILE *arq){
  struct usuario u3;
  int n,existe;
  existe = 0;
  n=0;
  arq = fopen("arquivo.dat", "r+b");
  while(n<TAMANHO_ARQUIVO){
    fseek(arq,n*sizeof(u3),0);
    fread(&u3,sizeof(u3),1,arq);
    if(u3.chave == chave){
      existe = 1;
    }
    n++;
  }
  fclose(arq);
  return existe;
}

int hash_calculo_duplo(int chave,int indice){
  int h1,h2,h;
  h1 = chave % TAMANHO_ARQUIVO;
  h2 = (chave/TAMANHO_ARQUIVO) % TAMANHO_ARQUIVO;
  if(h2==0){
    h2=1;
  }
  h = (h1 + indice*h2) % TAMANHO_ARQUIVO;
  return h;
}

void escreve_arquivo_duplo(FILE *arq){
  int n,chave,idade,hash_valor,hash_move,existe,anterior,indice;
  char name[20];
  struct usuario u1;
  struct usuario u2;

  scanf("%d",&chave);
  fgetc(stdin);
  fgets(name,20,stdin);
  strtok(name,"\n");
  scanf("%d",&idade);

  //Calculo do hash e teste se a chave ja existe no arquivo
  indice =0;
  hash_valor = hash_calculo_duplo(chave,indice);
  existe = chave_teste_duplo(chave,arq);

  if(!existe){
    arq = fopen("arquivo.dat", "r+b");
    fseek(arq,(hash_valor)*sizeof(u1),0);
    fread(&u1,sizeof(u1),1,arq);
    //Verifica se nao ocorre colisao (se ja tem uma chave na posicao)
    if(u1.chave == 0 || u1.chave ==-1){
      strcpy(u1.nome,name);
      u1.idade = idade;
      u1.chave = chave;
      u1.pointer = TAMANHO_ARQUIVO;
      fseek(arq,(hash_valor)*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);
      //Se ocorre colisao
    }else if(hash_valor == hash_calculo_duplo(u1.chave,0)){
      while(u1.chave != 0){
        indice++;
        fseek(arq,(hash_calculo_duplo(chave,indice))*sizeof(u1),0);
        fread(&u1,sizeof(u1),1,arq);
      }
      fseek(arq,(hash_calculo_duplo(chave,indice))*sizeof(u1),0);
      strcpy(u1.nome,name);
      u1.idade = idade;
      u1.chave = chave;
      u1.pointer = TAMANHO_ARQUIVO;
      fwrite(&u1,sizeof(u1),1,arq);
    }else{
      indice=0;
      fseek(arq,hash_calculo_duplo(u1.chave,indice)*sizeof(u1),0);
      fread(&u2,sizeof(u2),1,arq);
      while(u2.chave != 0 && u2.chave != -1){
        indice++;
        fseek(arq,(hash_calculo_duplo(u1.chave,indice))*sizeof(u1),0);
        fread(&u2,sizeof(u1),1,arq);
      }
      fseek(arq,(hash_calculo_duplo(u1.chave,indice))*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);

      fseek(arq,(hash_calculo_duplo(chave,0))*sizeof(u1),0);
      strcpy(u1.nome,name);
      u1.idade = idade;
      u1.chave = chave;
      u1.pointer = TAMANHO_ARQUIVO;
      fwrite(&u1,sizeof(u1),1,arq);
    }
  }
  fclose(arq);
}

void consulta_arquivo_duplo(FILE *arq){
  int chave,hash,indice,existe;
  struct usuario u1;

  indice = 0;
  existe = 0;
  scanf("%d",&chave);
  hash = hash_calculo_duplo(chave,indice);
  arq = fopen("arquivo.dat", "r+b");
  fseek(arq,hash*sizeof(u1),0);
  fread(&u1,sizeof(u1),1,arq);
  if(u1.chave == chave){
    existe = 1;
  }else{
    //Verifica se a chave esta encadeada a sua posicao de hash
    while(u1.chave != chave && u1.chave != 0){
      indice++;
      fseek(arq,hash_calculo_duplo(chave,indice)*sizeof(u1),0);
      fread(&u1,sizeof(u1),1,arq);
      if(u1.chave == chave){
        existe = 1;
      }
    }
  }
  if(existe == 1){
    printf("chave: %d\n",u1.chave);
    printf("%s\n",u1.nome);
    printf("%d\n",u1.idade);
  }else{
    printf("chave nao encontrada: %d\n",chave);
  }
  fclose(arq);
}

void remove_arquivo_duplo(FILE *arq){
  int key,hash,chave,existe,posi,indice;
  struct usuario u1,u2;
  existe = 0;
  indice = 0;
  scanf("%d",&chave);
  hash = hash_calculo_duplo(chave,indice);
  arq = fopen("arquivo.dat", "r+b");
  fseek(arq,hash*sizeof(u1),0);
  fread(&u1,sizeof(u1),1,arq);
  if(u1.chave == chave){
    existe = 1;
    fseek(arq,hash*sizeof(u1),0);
    strcpy(u1.nome,"vazio");
    u1.idade = 0;
    u1.chave = -1;
    u1.pointer = TAMANHO_ARQUIVO;
    fwrite(&u1,sizeof(u1),1,arq);
  }else{
    while(u1.chave != chave && u1.chave != 0){
      indice++;
      fseek(arq,hash_calculo_duplo(chave,indice)*sizeof(u1),0);
      fread(&u1,sizeof(u1),1,arq);
      if(u1.chave == chave){
        existe = 1;
      }
    }
    if(existe){
      fseek(arq,hash_calculo_duplo(chave,indice)*sizeof(u1),0);
      strcpy(u1.nome,"vazio");
      u1.idade = 0;
      u1.chave = -1;
      u1.pointer = TAMANHO_ARQUIVO;
      fwrite(&u1,sizeof(u1),1,arq);
    }else{
      printf("chave nao encontrada: %d\n",chave);
    }
  }
  fclose(arq);
}

int media_acesso_duplo(FILE *arq){
  int n,qtd,soma,chaves,indice;
  struct usuario u1,u2;
  float media;

  arq = fopen("arquivo.dat", "r+b");
  qtd = 1;
  soma = 0;
  chaves = 0;
  indice = 0;
  for(n=0;n<TAMANHO_ARQUIVO;n++){
    fseek(arq,n*sizeof(u1),0);
    fread(&u1,sizeof(u1),1,arq);
    if(n != hash_calculo_duplo(u1.chave,indice) && u1.chave != 0 && u1.chave != -1){
      fseek(arq,hash_calculo_duplo(u1.chave,indice)*sizeof(u2),0);
      fread(&u2,sizeof(u2),1,arq);

      while(u2.chave != u1.chave){
        indice++;
        fseek(arq,hash_calculo_duplo(u1.chave,indice)*sizeof(u1),0);
        fread(&u2,sizeof(u2),1,arq);
        qtd++;
      }
    }
    if(u1.chave!= 0 && u1.chave!= -1){
      chaves = chaves+1;
      soma = soma+qtd;
    }
    qtd = 1;
    indice = 0;
  }
  media = (float) soma/chaves;
  printf("%.1f\n",media);
  fclose(arq);
}

void imprime_encadeado_duplo(FILE *arq){
  int n;
  struct usuario u;

  n = 0;
  arq = fopen("arquivo.dat", "r+b");
  while(n < TAMANHO_ARQUIVO){
    fseek(arq,n*sizeof(u),0);
    fread(&u,sizeof(u),1,arq);
    if(strcmp(u.nome,"vazio")==0){
        printf("%d: vazio\n",n);
    }else{
        printf("%d: %d %s %d\n",n,u.chave,u.nome,u.idade);
    }
    n++;
  }
  fclose(arq);
}

//Funcao testa se a chave a ser inserida já existe no arquivo
int chave_teste(int chave, FILE *arq){
  struct usuario u3;
  int n,existe;
  existe = 0;
  n = 0;
  arq = fopen("arquivo.dat", "r+b");
  while(n < TAMANHO_ARQUIVO){
    fseek(arq,n*sizeof(u3),0);
    fread(&u3,sizeof(u3),1,arq);
    if(u3.chave == chave){
      existe = 1;
    }
    n++;
  }
  fclose(arq);
  return existe;
}

//Funcao que calcula o hash a pratir do valor da chave
int hash_calculo(int chave){
  int h;
  h = chave % TAMANHO_ARQUIVO;
  return h;
}



void escreve_arquivo(FILE *arq){
  int n,chave,idade,hash_valor,hash_move,existe,anterior;
  char name[20];
  struct usuario u1;
  struct usuario u2;

  scanf("%d",&chave);
  fgetc(stdin);
  fgets(name,20,stdin);
  strtok(name,"\n");
  scanf("%d",&idade);
  //Calculo do hash e teste se a chave ja existe no arquivo
  hash_valor = hash_calculo(chave);
  existe = chave_teste(chave,arq);

  if(!existe){
    arq = fopen("arquivo.dat", "r+b");
    fseek(arq,(hash_valor)*sizeof(u1),0);
    fread(&u1,sizeof(u1),1,arq);
    //Verifica se nao ocorre colisao (se ja tem uma chave na posicao)
    if(u1.chave == 0){
      strcpy(u1.nome,name);
      u1.idade = idade;
      u1.chave = chave;
      u1.pointer = TAMANHO_ARQUIVO;
      fseek(arq,(hash_valor)*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);
      //Se ocorre colisao
    }else if(hash_valor == hash_calculo(u1.chave)){
      n = 10;
      u2 = u1;
      //Procura por posicao disponivel
      while(u2.chave != 0){
        fseek(arq,n*sizeof(u2),0);
        fread(&u2,sizeof(u2),1,arq);
        n--;
      }
      //Salva a estrutura na posicao encontrada
      strcpy(u2.nome,name);
      u2.idade = idade;
      u2.chave = chave;
      u2.pointer = TAMANHO_ARQUIVO;
      fseek(arq,(n+1)*sizeof(u2),0);
      fwrite(&u2,sizeof(u2),1,arq);

      //Procura por ultima estrutura na cadeia de encadeamento
      //referente ao hash que sofreu colisao
      anterior=hash_valor;
      while(u1.pointer < TAMANHO_ARQUIVO){
        anterior = u1.pointer;
        fseek(arq,u1.pointer*sizeof(u1),0);
        fread(&u1,sizeof(u1),1,arq);
      }
      //Acrescenta na ultima posicao o valor do ponteiro para a nova estrutura
      fseek(arq,anterior*sizeof(u1),0);
      fread(&u1,sizeof(u1),1,arq);
      u1.pointer = (n+1);
      fseek(arq,anterior*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);
    }else{
      //Caso seja necessário mover arquivo para realizar insercao
      u2 = u1;
      n = 10;
      //Procura por uma posicao vazia
      while(u2.chave != 0){
        fseek(arq,n*sizeof(u2),0);
        fread(&u2,sizeof(u2),1,arq);
        n--;
      }
      //Move o registro ja no arquivo para a posicao vazia encontrada
      fseek(arq,(n+1)*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);
      //Procura posicao que apontava para o registro movido
      hash_move = hash_calculo(u1.chave);
      fseek(arq,hash_move*sizeof(u1),0);
      fread(&u1,sizeof(u1),1,arq);
      anterior = hash_move;
      while(u1.pointer != hash_valor){
        anterior = u1.pointer;
        fseek(arq,u1.pointer*sizeof(u1),0);
        fread(&u1,sizeof(u1),1,arq);
      }
      //modifica o pointer do registro encontrado
      u1.pointer = (n+1);
      fseek(arq,anterior*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);
      //Salva o novo registro em sua posicao correta no arquivo
      strcpy(u2.nome,name);
      u2.idade = idade;
      u2.chave = chave;
      u2.pointer = TAMANHO_ARQUIVO;
      fseek(arq,hash_valor*sizeof(u2),0);
      fwrite(&u2,sizeof(u2),1,arq);
    }
    fclose(arq);
  }else{
    printf("chave ja existente %d\n",chave);
  }
}

//Funcao que verifica se uma chave ja existe no arquivo
void consulta_arquivo(FILE *arq){
  int chave,existe,hash;
  struct usuario u1;

  scanf("%d",&chave);
  hash = hash_calculo(chave);
  arq = fopen("arquivo.dat", "r+b");
  fseek(arq,hash*sizeof(u1),0);
  fread(&u1,sizeof(u1),1,arq);
  existe = 0;
  if(u1.chave == chave){
    existe = 1;
  }else{
    //Verifica se a chave esta encadeada a sua posicao de hash
    while(u1.chave != chave && u1.pointer != TAMANHO_ARQUIVO){
      fseek(arq,u1.pointer*sizeof(u1),0);
      fread(&u1,sizeof(u1),1,arq);
      if(u1.chave == chave){
        existe = 1;
      }
    }
  }
  if(existe == 1){
    printf("chave: %d\n",u1.chave);
    printf("%s\n",u1.nome);
    printf("%d\n",u1.idade);
  }else{
    printf("chave nao encontrada: %d\n",chave);
  }
  fclose(arq);
}

void remove_arquivo(FILE *arq){
  int key,hash,chave,existe,posi;
  struct usuario u1,u2;

  scanf("%d",&chave);
  hash = hash_calculo(chave);
  arq = fopen("arquivo.dat", "r+b");
  fseek(arq,hash*sizeof(u1),0);
  fread(&u1,sizeof(u1),1,arq);
  if(u1.chave == chave){
    if(u1.pointer != TAMANHO_ARQUIVO){
      fseek(arq,u1.pointer*sizeof(u1),0);
      fread(&u2,sizeof(u2),1,arq);
      fseek(arq,hash*sizeof(u1),0);
      fwrite(&u2,sizeof(u2),1,arq);

      fseek(arq,u1.pointer*sizeof(u1),0);
      strcpy(u1.nome,"vazio");
      u1.idade = 0;
      u1.chave = 0;
      u1.pointer = TAMANHO_ARQUIVO;
      fwrite(&u1,sizeof(u1),1,arq);
    }else{
      fseek(arq,hash*sizeof(u1),0);
      strcpy(u1.nome,"vazio");
      u1.idade = 0;
      u1.chave = 0;
      u1.pointer = TAMANHO_ARQUIVO;
      fwrite(&u1,sizeof(u1),1,arq);
    }
  }else{
    existe = 0;
    while(u1.chave != chave && u1.pointer != TAMANHO_ARQUIVO){
      posi = u1.pointer;
      fseek(arq,u1.pointer*sizeof(u1),0);
      fread(&u1,sizeof(u1),1,arq);
      if(u1.chave == chave){
        existe = 1;
        if(u1.pointer != TAMANHO_ARQUIVO){
          fseek(arq,u1.pointer*sizeof(u1),0);
          fread(&u2,sizeof(u2),1,arq);
          fseek(arq,posi*sizeof(u1),0);
          fwrite(&u2,sizeof(u2),1,arq);

          fseek(arq,u1.pointer*sizeof(u1),0);
          strcpy(u1.nome,"vazio");
          u1.idade = 0;
          u1.chave = 0;
          u1.pointer = TAMANHO_ARQUIVO;
          fwrite(&u1,sizeof(u1),1,arq);
        }else{
          fseek(arq,hash*sizeof(u1),0);
          strcpy(u1.nome,"vazio");
          u1.idade = 0;
          u1.chave = 0;
          u1.pointer = TAMANHO_ARQUIVO;
          fwrite(&u1,sizeof(u1),1,arq);
        }
      }
    }
    if(existe==0){
      printf("chave nao encontrada: %d\n",chave);
    }
  }
  fclose(arq);
}

//Calcula a media de acessos
int media_acesso(FILE *arq){
  int n,qtd,soma,chaves;
  struct usuario u1,u2;
  float media;

  arq = fopen("arquivo.dat", "r+b");
  qtd = 1;
  soma = 0;
  chaves = 0;
  for(n=0;n<TAMANHO_ARQUIVO;n++){
    fseek(arq,n*sizeof(u1),0);
    fread(&u1,sizeof(u1),1,arq);
    if(n != hash_calculo(u1.chave) && u1.chave != 0){
      fseek(arq,hash_calculo(u1.chave)*sizeof(u2),0);
      fread(&u2,sizeof(u2),1,arq);

      while(u2.chave != u1.chave){
        fseek(arq,u2.pointer*sizeof(u1),0);
        fread(&u2,sizeof(u2),1,arq);
        qtd++;
      }
    }
    if(u1.chave!=0){
      chaves = chaves+1;
      soma = soma+qtd;
    }
    qtd = 1;
  }
  media = (float) soma/chaves;
  printf("%.1f\n",media);
  fclose(arq);
}

void imprime_encadeado(FILE *arq){
  int n;
  struct usuario u;

  n = 0;
  arq = fopen("arquivo.dat", "r+b");
  while(n < TAMANHO_ARQUIVO){
    fseek(arq,n*sizeof(u),0);
    fread(&u,sizeof(u),1,arq);
    if(strcmp(u.nome,"vazio")==0){
      if(u.pointer != TAMANHO_ARQUIVO){
        printf("%d: vazio %d\n",n,u.pointer);
      }else{
        printf("%d: vazio nulo\n",n);
      }
    }else{
      if(u.pointer != TAMANHO_ARQUIVO){
        printf("%d: %d %s %d %d\n",n,u.chave,u.nome,u.idade,u.pointer);
      }else{
        printf("%d: %d %s %d nulo\n",n,u.chave,u.nome,u.idade);
      }
    }

    n++;
  }
  fclose(arq);
}

struct usuario inicializa_encadeado(FILE *arq){
  int n,chave,idade;
  char name[20];
  struct usuario u;

  //Cria uma estrutura com valores 'vazios' e com essa Estrutura
  //inicialia cada posicao do arquivo
  strcpy(u.nome,"vazio");
  u.idade = 0;
  u.chave = 0;
  u.pointer = TAMANHO_ARQUIVO;

  n = 1;
  while(n <= TAMANHO_ARQUIVO){
      fwrite(&u,sizeof(u),1,arq);
      n++;
  }
}

struct usuario inicializa_duplo(FILE *arq){
  int n,chave,idade;
  char name[20];
  struct usuario u;

  //Cria uma estrutura com valores 'vazios' e com essa Estrutura
  //inicialia cada posicao do arquivo
  strcpy(u.nome,"vazio");
  u.idade = 0;
  u.chave = 0;

  n = 1;
  while(n <= TAMANHO_ARQUIVO){
      fwrite(&u,sizeof(u),1,arq);
      n++;
  }
}

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
