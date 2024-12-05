#ifndef _ARVORE_RUBRO_NEGRO_H
  #define _ARVORE_RUBRO_NEGRO_H

  //Padrão definido para representar as cores vermelho e preto
  #include <stdbool.h>
  #define BLACK 0
  #define RED 1

  //A estrutura da Árvore Rubro Negro é definida como seu acrônimo
  typedef struct arn_ ARN;
 
  /*
  Para criar uma árvore não é preciso passar parâmetros, apenas relacionar um ponteiro ARN a função.
  Se a alocação não for sucedida, retorna NULL.
  Caso contário retorna um ponteiro para ARN atribuindo a raiz como NULL.
  */
  ARN *arn_criar (void); 
  
  /*
  Passa inserir uma chave basta passar a árvore de destino e o valor da chave.
  Não é possível inserir dois itens de mesma chave.
  Retorna "true" se a inserção foi concluída com sucesso, "false" caso não.
  */
  bool arn_inserir (ARN *T, int chave);

  /*
  A remoção é similar a inserção em seus parâmetos, o retorno é um booleano que diz 
  se foi possível ou não remover a chave removida. Quando ela não está na árvore retorna NULL.
  */
  bool arn_remover(ARN *T, int chave);  

  /*
  Quando passada uma árvore que existe, essa função a imprimirá em pré-ordem para uma melhor vizualização
  de como estão dispostos os elementos nessa estrutura.
  */
  void arn_imprimir (ARN *T);
  
  /*
  Apaga a árvore passada de parâmetro, deletando devidamente todos os seus nós e a estrutura ARN em si
  */
  void arn_apagar (ARN **T);

  /*
  Colocando a árvore que deseja ser buscada e a chave do item como parametros, é retornado um ponteiro para
  ele sem que seja retirado da árvore.
  */
  bool arn_pertence(ARN *T, int chave);

  /*
  Apenas diz se a árvore dada está vazia ou não
  */
  bool arn_vazia(ARN *T);
  
  /*Função para mesclar os elementos de duas árvores*/
  ARN *arn_uniao(ARN *Result, ARN *A, ARN *B);

  /*Função para criar uma arvore a partir dos seus elementos identicos de duas árvores*/
  void arn_intersec(ARN* intersection, ARN *a, ARN *b);
  
#endif
