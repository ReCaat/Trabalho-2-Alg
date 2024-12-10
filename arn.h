/*
Referente ao header do conjunto 2 - Estrutura Árvore rubro-negro (variação left-leaning)
Aqui estão as funções principais devidamente comentadas e disponíveis para o usuário usar

Autoria: 
João Victor Alonso de Mello N°USP - 10951790
Renan Banci Catarin N°USP - 14658181
*/


#ifndef _ARVORE_RUBRO_NEGRO_H
  #define _ARVORE_RUBRO_NEGRO_H

  //Padrão definido para representar as cores vermelho e preto
  #include <stdbool.h>
  #define BLACK 0
  #define RED 1

  //A estrutura da Árvore Rubro Negro é definida como seu acrônimo
  typedef struct arn_ ARN;
  
  /*
  Parametros frequêntes:
  T - A árvore a sofrer a operação
  chave - valor a ser adicionado, removido ou buscado
  */

  /*
  Para criar uma árvore não é preciso passar parâmetros, apenas relacionar um ponteiro ARN a função.
  Se a alocação não for sucedida, retorna NULL.
  Caso contário retorna um ponteiro para ARN atribuindo a raiz como NULL.
  */
  ARN *arn_criar (void); 
  
  /*
  Insere uma determinada chave na árvore que foi passada como parâmetro. Isso porque essa árvore representa um conjunto
  Não é possível inserir dois itens de mesma chave.
  Retorna "true" se a inserção foi concluída com sucesso, "false" caso não.
  */
  bool arn_inserir (ARN *T, int chave);

  /*
  Remove uma determinada chave da árvore que foi passada como parâmetro.
  Retorna um booleano que diz se a chave foi removida ou não.
  Chaves não são removidas quando não estão presentes na árvore ou quando a árvore não existe ou está vazia.
  */
  bool arn_remover(ARN *T, int chave);  

  /*
  Verifica se uma determinada chave pertence ou não a uma determinada árvore.
  Se pertencer retorna true, e false caso contrário.
  */
  bool arn_pertence(ARN *T, int chave);
  
  /*
  Se a árvore passada como parâmetro existir, imprimirá seus valores em pré-ordem para melhor vizualização da estrutura.
  */
  void arn_imprimir (ARN *T);
  
  /*
  Apaga a árvore passada de parâmetro, deletando devidamente todos os seus nós e a estrutura ARN em si
  */
  void arn_apagar (ARN *T);

  /*
  Diz se a árvore dada está vazia (true) ou não (false),
  se não existir retorna true.
  */
  bool arn_vazia(ARN *T);
  
  /*
  Une os elementos de duas árvores dadas em uma nova
  Note que ela retorna um ponteiro para a árvore que sairá como resultado.
  Logo, é preciso relacionar a árvorenova tanto como parametro quanto como saida
  */
  void arn_uniao(ARN *Result, ARN *A, ARN *B);

  /*
  Relaciona os itens de duas árvores dadas, quando um pertence a ambas,o adiciona em uma nova, também dada.
  Diferentemente da função de união, ela não tem a necessidade de retornar um ponteiro para a árvore de intersecção 

  */
  void arn_intersect(ARN* intersection, ARN *A, ARN *B);
  
#endif
