#ifndef _SET_H
#define _SET_H

  #include "avl.h"
  #include "arn.h" //São as interfaces dos TADs que serão 
  //#include "ED2.h" //utilizados para armazenar os Dados do Set.
                   //i.e, Set é um TAD que usa outros TADs. 
                   //Pode trocar os nomes! ;-)

 typedef struct set SET;

  SET *set_criar(unsigned char tipo);
  bool set_pertence(SET *A, int elemento);
  bool set_inserir (SET *s, int elemento);
  bool set_remover(SET *s, int elemento);
  void set_apagar(SET **s);
  void set_imprimir(SET *s);
  SET *set_uniao(SET *A, SET *B);
  SET *set_interseccao(SET *A, SET *B);
  
#endif