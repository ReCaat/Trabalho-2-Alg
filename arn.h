#ifndef _ARVORE_RUBRO_NEGRO_H
  #define _ARVORE_RUBRO_NEGRO_H

  typedef struct arn ARN;
 
  ARN *arn_criar (void); 
  bool arn_inserir (ARN *T, int chave);
  void arn_imprimir (ARN *T);
  void arn_apagar (ARN **T);
  bool arn_remover(ARN *T, int chave);  
  int *arn_busca(ARN *T, int chave);
  bool arn_vazia(ARN *T);
  void arn_merge(AVL *uniao, AVL *a, AVL *b);
  void arn_intersect(AVL* intersection, AVL *a, AVL *b);
#endif