#include "set.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  SET *A, *B;
  int n_a, n_b, x;
  int op;
  int tipo;

  scanf("%d", &tipo); /*0 -> ED1, 1 -> ED2*/

  A = set_criar(tipo);
  B = set_criar(tipo);

  scanf("%d %d", &n_a, &n_b);

  for(int i=0; i<n_a; i++){
    scanf("%d",&x);
    set_inserir(A, x);

  }

  for(int i=0; i<n_b; i++){
    scanf("%d",&x);
    set_inserir(B, x);

  }

  scanf("%d", &op);

  switch(op){
    case 1: {
              //pertence
              int elemento;
              scanf("%d", &elemento);
              if (set_pertence(A, elemento)) printf("Pertence.\n");
              else printf("Nao pertence.\n");
              break;
    }
    case 2: {
              //união
              SET *uniao = set_uniao(A, B);
              set_imprimir(uniao);
              set_apagar(&uniao);
              break;
    }
    case 3: {
              //intersecção
              SET *interseccao = set_interseccao(A, B);
              set_imprimir(interseccao);
              set_apagar(&interseccao);
              break;
    }
    case 4: {
              //remoção
              int elemento;
              scanf("%d", &elemento);
              set_remover(A, elemento);
              set_imprimir(A);
              break;
    }
  }
  //set_imprimir(A);
  set_apagar(&A);
  set_apagar(&B);  
  return 0;
}
