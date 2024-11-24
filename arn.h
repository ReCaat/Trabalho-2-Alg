#ifndef _ARVORE_RUBRO_NEGRO_H
  #define _ARVORE_RUBRO_NEGRO_H

  #include <stdbool.h>
  #define BLACK 0
  #define RED 1

  typedef struct arn_ ARN;
 
  ARN *arn_criar (void); 
  bool arn_inserir (ARN *T, int chave);
  void arn_imprimir (ARN *T);
  void arn_apagar (ARN **T);
  bool arn_remover(ARN *T, int chave);  
  int *arn_busca(ARN *T, int chave);
  bool arn_vazia(ARN *T);
  int arn_get_tamanho(ARN *T);
  
  ARN *arn_uniao(ARN *Result, ARN *A, ARN *B);

#endif
