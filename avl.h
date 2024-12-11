/*
Referente ao header do conjunto 1 - Estrutura Árvore AVL
Aqui estão as funções principais devidamente comentadas e disponíveis para o usuário usar

Autoria: 
João Victor Alonso de Mello / N°USP - 10951790
Renan Banci Catarin / N°USP - 14658181
*/

#include <stdlib.h>
#include <stdbool.h>

typedef struct avl_wrapper AVL;

/*
Parametros frequêntes:
arvore - A árvore a sofrer a operação
valor - chave a ser adicionada, removida ou buscada
*/

/*
  Para criar uma árvore não é preciso passar parâmetros, apenas relacionar um ponteiro ARN a função.
  Se a alocação não for sucedida, retorna NULL.
  Caso contário retorna um ponteiro para ARN atribuindo a raiz como NULL.
*/
AVL *arvore_criar();

/*
  Insere uma determinada chave na árvore que foi passada como parâmetro. 
  Não é possível inserir duas vezes a mesma chave. Isso porque essa árvore representa um conjunto.
  Retorna "true" se a inserção foi concluída com sucesso, "false" caso não.
*/
bool arvore_inserir(AVL *arvore, int valor);

/*
  Remove uma determinada chave da árvore que foi passada como parâmetro.
  Retorna um booleano que diz se a chave foi removida ou não.
  Chaves não são removidas quando não estão presentes na árvore ou quando a árvore não existe ou está vazia.
*/
bool arvore_remover(AVL *arvore, int valor);

/*
  Verifica se uma determinada chave pertence ou não a uma determinada árvore.
  Se pertencer retorna true, e false caso contrário.
*/
bool arvore_pertence(AVL *arvore, int valor);

/*
  Se a árvore passada como parâmetro existir, imprimirá seus valores em pré-ordem para melhor vizualização da estrutura.
*/
void arvore_imprimir(AVL *arvore);

/*
  Apaga a árvore passada de parâmetro, deletando devidamente todos os seus nós e a estrutura ARN em si
*/
void arvore_apagar(AVL *arvore);

/*
  Une os elementos de duas árvores dadas em uma nova
  Note que ela retorna um ponteiro para a árvore que sairá como resultado.
  Logo, é preciso relacionar a árvorenova tanto como parametro quanto como saida
*/
void arvore_merge(AVL *uniao, AVL *a, AVL *b);

/*
  Relaciona os itens de duas árvores dadas, quando um pertence a ambas,o adiciona em uma nova, também dada.
  Diferentemente da função de união, ela não tem a necessidade de retornar um ponteiro para a árvore de intersecção 
*/
void arvore_intersect(AVL* intersection, AVL *a, AVL *b);

/*
  Retorna o valor da altura da árvore passada como parâmetro.
*/
int arvore_get_tamanho(AVL *arvore);
