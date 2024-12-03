#include "avl.h"
#include <stdlib.h>
#include <stdio.h>
//este codigo implementa a E.D. arvore Adelson-Velsky and Landis (AVL)

struct avl_ {
    struct avl_ *left, *right; // filho esquerdo e direito de um no da arvore
    int valor; //chave
    int height;
};
typedef struct avl_ ARV;

struct avl_wrapper{ //usado para poder tratar arvores de tamanho 0 sem a necessidade de um if case adicional
    ARV *arvore;
    int size;
};

void recalcular_altura(ARV *arv);

int max(int a, int b){
    return (a > b ? a:b);
}

AVL *arvore_criar(){
    AVL *arvore = calloc(1, sizeof(AVL));
    return arvore;
}

ARV *criar_no(int item){ //uma arvore de um no tem altura 0 nesta implementacao
    ARV *arv = malloc(sizeof(ARV));
    arv->height = 0;
    arv->valor = item;
    arv->left = NULL; arv->right = NULL;
    return arv;
}

//faz uma rotacao para a esquerda de x e seu filho direito y (presume-se que x possui filho direito)
//X passa a ser filho esquerdo de y e o novo filho direito de x passa a ser o filho esquerdo de y
//apos a rotacao ser feita, recalcula-se a altura da arvore
ARV* left_rotation(ARV* x){ 
    ARV* y = x->right;
    x->right = y->left;
    y->left = x;
    recalcular_altura(x);
    recalcular_altura(y);
    return y;
}

//esta rotacao eh a versao espelhada da para a esquerda, basta inverter esquerda por direita e vice-versa
ARV* right_rotation(ARV* x){
    ARV* y = x->left;
    x->left = y->right;
    y->right = x;
    recalcular_altura(x);
    recalcular_altura(y);
    return y;
}

//calcula o teor de balanceamento a partir das alturas ja salvas nos nos
int height_difference(ARV* atual){
    int left_height = atual->left == NULL ? -1 : atual->left->height;
    int right_height = atual->right == NULL ? -1 : atual->right->height;
    return right_height - left_height;
}

//insere o no "novo" na arvore "atual"
//Para encontrar o no, percorre-se a arvore atraves de uma busca binaria ate chegar em um no folha.
//este no folha passara a ser o pai do novo no
//depois, recalcula-se as alturas e sao feitas rotacoes na arvore caso ela tenha se tornado desbalanceada apos a insercao
ARV* helper_arvore_inserir(ARV* atual, ARV* novo, bool *adicionado){
    if (atual == NULL) return novo;
    if (atual->valor == novo->valor) {
        *adicionado = false;
        free(novo);
        return atual; //evita valores duplicados
    }
    if (atual->valor > novo->valor){
        atual->left = helper_arvore_inserir(atual->left, novo, adicionado);
    }
    else {
        atual->right = helper_arvore_inserir(atual->right, novo, adicionado);
    }
    recalcular_altura(atual);
    int diff = height_difference(atual); 
    if (diff >= 2){ //pesada para a direita
        if (height_difference(atual->right) < 0){ //caso necessita de rotacao dupla
            atual->right = right_rotation(atual->right);
        }
        atual = left_rotation(atual);
    }
    else if (diff <= -2){ //pesada para a esquerda
        if (height_difference(atual->left) > 0){//caso necessita de rotacao dupla
            atual->left = left_rotation(atual->left);
        }
        atual = right_rotation(atual);
    }
    return atual;
}
//wraper da funcao acima, retorna 1 se a insercao foi feita com sucesso e 0 se nao.
bool arvore_inserir(AVL* avl, int item){
    bool adicionado = true;
    avl->arvore = helper_arvore_inserir(avl->arvore, criar_no(item), &adicionado);
    avl->size += adicionado;
    return adicionado;
}

//encontra o no com a menor chave da arvore
ARV* find_min(ARV *arv){
    return (arv->left == NULL ? arv : find_min(arv->left));
}

//recalcula a altura, levando em conta os casos em que um ou mais filhos sao nulos
void recalcular_altura(ARV *arv){
    if (arv->left == arv->right) arv->height = 0;
    else if (arv->left == NULL) arv->height = arv->right->height + 1;
    else if (arv->right == NULL) arv->height = arv->left->height + 1;
    else arv->height = max(arv->left->height, arv->right->height) + 1;
}

//remove um no da arvore por seu valor e reconecta os nos restantes de maneira a manter o balanceamento e estrutura de busca binaria
ARV *helper_arvore_remover(ARV* avl, int item, bool *removido){
    if (avl == NULL) return NULL;
    if (avl->valor == item){
        *removido = true;
        if (avl->left == avl->right){ //o no nao tem filhos e pode ser deletado sem problemas
            free(avl);
            return NULL;
        }
        else if(avl->left == NULL || avl->right == NULL){ //o no so tem um filho
            ARV *tmp = (avl->left == NULL) ? avl->right : avl->left; 
            free(avl);
            return tmp; //o filho passa a ocupar a posicao do no
        }
        else{ //o no tem dois filhos
            int tmp = find_min(avl->right)->valor;
            avl->right = helper_arvore_remover(avl->right, tmp, removido);
            recalcular_altura(avl);
            avl->valor = tmp; // o valor do no passa a ser a menor chave da subarvore que comeca com este no
        }
    }
    else if (avl->valor > item){ //busca binaria
        avl->left = helper_arvore_remover(avl->left, item, removido);
    }
    else {//busca binaria
        avl->right = helper_arvore_remover(avl->right, item, removido);
    }
    recalcular_altura(avl);
    if (height_difference(avl) >= 2){ //arvore pesada para a direita
        if (height_difference(avl->right) < 0){//rotacao dupla
            avl->right = right_rotation(avl->right);
        }
        avl = left_rotation(avl);
    }
    else if (height_difference(avl) <= -2){ //arvore pesada para a esquerda
        if (height_difference(avl->left) > 0){//rotacao dupla
            avl->left = left_rotation(avl->left);
        }
        avl = right_rotation(avl);
    }
    return avl;
}

//wrapper para a funcao acima, retorna 1 caso um no tenha sido removido e 0 se nao
bool arvore_remover(AVL *arv, int item){
    bool removido = false;
    arv->arvore = helper_arvore_remover(arv->arvore, item, &removido);
    arv->size -= removido;
    return removido;
}

//desaloca todos os no da arvore
void helper_arvore_apagar(ARV *arv){
    if (arv == NULL) return;
    helper_arvore_apagar(arv->left);
    helper_arvore_apagar(arv->right);
    free(arv);
}

//wrapper pra funcao acima, desaloca o wrapper AVL alem dos nos.
void arvore_apagar(AVL *arv){
    helper_arvore_apagar(arv->arvore);
    free(arv);
}

//imprime a arvore em ordem
void helper_arvore_imprimir(ARV *arv){
    if (!arv) return;
    helper_arvore_imprimir(arv->left);
    printf("%d ", arv->valor);
    helper_arvore_imprimir(arv->right);
}

//wrapper pra funcao acima, adiciona um \n no final para facilitar a leitura
void arvore_imprimir(AVL *avl){
    helper_arvore_imprimir(avl->arvore);
    printf("\n");
}

//procura uma chave na arvore por busca binaria e retorna 1 se a encontrou
bool pertence_helper(ARV *arv, int valor){
    if (arv == NULL) return 0;
    if (arv->valor == valor) return 1;
    if (arv->valor > valor) return pertence_helper(arv->left, valor);
    else return pertence_helper(arv->right, valor);
}

//wrapper pra funcao acima
bool arvore_pertence(AVL *avl, int valor){
    return pertence_helper(avl->arvore, valor);
}

int arvore_get_tamanho(AVL *avl){
    return avl->size;
}

//adiciona todos os nos da arvore "original" na arvore "nova"
void insert_tree(AVL *nova, ARV *original){
    if (original == NULL || nova == NULL) return;
    arvore_inserir(nova, original->valor);
    insert_tree(nova, original->left);
    insert_tree(nova, original->right);
}

//realiza a uniao de a e b, guardando o resultado em "uniao"
void arvore_merge(AVL* uniao, AVL *a, AVL *b){
    insert_tree(uniao, a->arvore);
    insert_tree(uniao, b->arvore);
}

//calcula a interseccao entre b e a ("no_de_a"), armazenando o resultado em "interseccao"
void interseccao_helper(AVL *interseccao, AVL *b, ARV *no_de_a){
    if (interseccao == NULL || no_de_a == NULL) return;
    if (arvore_pertence(b, no_de_a->valor)) arvore_inserir(interseccao, no_de_a->valor);
    interseccao_helper(interseccao, b, no_de_a->left);
    interseccao_helper(interseccao, b, no_de_a->right);
}

//wrapper pra funcao acima
//otimiza o calculo da interseccao garantindo que sao checados sempre os nos da arvore de menor tamanho
void arvore_intersect(AVL* intersection, AVL *a, AVL *b){
    if (a->size < b->size){
        interseccao_helper(intersection, b, a->arvore);
    }
    else{
        interseccao_helper(intersection, a, b->arvore);
    }
}