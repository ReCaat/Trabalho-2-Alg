#include <stdlib.h>
#include <stdbool.h>
typedef struct avl_wrapper AVL;
AVL *arvore_criar();
bool arvore_inserir(AVL *arvore, int valor);
bool arvore_remover(AVL *arvore, int valor);
void arvore_apagar(AVL *arvore);
void arvore_imprimir(AVL *arvore);
bool arvore_pertence(AVL *arvore, int valor);
AVL *arvore_merge(AVL *uniao, AVL *a, AVL *b);
int arvore_get_tamanho(AVL *arvore);