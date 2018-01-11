#include "duplo.h"

int chave_teste_duplo(int chave,FILE *arq){
  struct usuariod u3;
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
  struct usuariod u1;
  struct usuariod u2;

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
    if(u1.chave == -2 || u1.chave ==-1){
      strcpy(u1.nome,name);
      u1.idade = idade;
      u1.chave = chave;
      fseek(arq,(hash_valor)*sizeof(u1),0);
      fwrite(&u1,sizeof(u1),1,arq);
      //Se ocorre colisao
    }else if(hash_valor == hash_calculo_duplo(u1.chave,0)){
      while(u1.chave != -2){
        indice++;
        fseek(arq,(hash_calculo_duplo(chave,indice))*sizeof(u1),0);
        fread(&u1,sizeof(u1),1,arq);
      }
      fseek(arq,(hash_calculo_duplo(chave,indice))*sizeof(u1),0);
      strcpy(u1.nome,name);
      u1.idade = idade;
      u1.chave = chave;
      fwrite(&u1,sizeof(u1),1,arq);
    }else{
      indice=0;
      fseek(arq,hash_calculo_duplo(u1.chave,indice)*sizeof(u1),0);
      fread(&u2,sizeof(u2),1,arq);
      while(u2.chave != -2 && u2.chave != -1){
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
      fwrite(&u1,sizeof(u1),1,arq);
    }
  }
  fclose(arq);
}

void consulta_arquivo_duplo(FILE *arq){
  int chave,hash,indice,existe;
  struct usuariod u1;

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
    while(u1.chave != chave && u1.chave != -2){
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
  struct usuariod u1,u2;
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
      fwrite(&u1,sizeof(u1),1,arq);
    }else{
      printf("chave nao encontrada: %d\n",chave);
    }
  }
  fclose(arq);
}

int media_acesso_duplo(FILE *arq){
  int n,qtd,soma,chaves,indice;
  struct usuariod u1,u2;
  float media;

  arq = fopen("arquivo.dat", "r+b");
  qtd = 1;
  soma = 0;
  chaves = 0;
  indice = 0;
  for(n=0;n<TAMANHO_ARQUIVO;n++){
    fseek(arq,n*sizeof(u1),0);
    fread(&u1,sizeof(u1),1,arq);
    if(n != hash_calculo_duplo(u1.chave,indice) && u1.chave != -2 && u1.chave != -1){
      fseek(arq,hash_calculo_duplo(u1.chave,indice)*sizeof(u2),0);
      fread(&u2,sizeof(u2),1,arq);

      while(u2.chave != u1.chave){
        indice++;
        fseek(arq,hash_calculo_duplo(u1.chave,indice)*sizeof(u1),0);
        fread(&u2,sizeof(u2),1,arq);
        qtd++;
      }
    }
    if(u1.chave!= -2 && u1.chave!= -1){
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
  struct usuariod u;

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

struct usuariod inicializa_duplo(FILE *arq){
  int n,chave,idade;
  char name[20];
  struct usuariod u;

  //Cria uma estrutura com valores 'vazios' e com essa Estrutura
  //inicialia cada posicao do arquivo
  strcpy(u.nome,"vazio");
  u.idade = 0;
  u.chave = -2;

  n = 1;
  while(n <= TAMANHO_ARQUIVO){
      fwrite(&u,sizeof(u),1,arq);
      n++;
  }
}
