#include "avl.h"
#include <stdlib.h>
#include <stdio.h>

struct avl_ {
    struct avl_ *left, *right;
    int valor;
    int height;
};
typedef struct avl_ ARV;

struct avl_wrapper{
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

ARV *criar_no(int item){
    ARV *arv = malloc(sizeof(ARV));
    arv->height = 0;
    arv->valor = item;
    arv->left = NULL; arv->right = NULL;
    return arv;
}

ARV* left_rotation(ARV* x){
    ARV* y = x->right;
    x->right = y->left;
    y->left = x;
    recalcular_altura(x);
    recalcular_altura(y);
    return y;
}

ARV* right_rotation(ARV* x){
    ARV* y = x->left;
    x->left = y->right;
    y->right = x;
    recalcular_altura(x);
    recalcular_altura(y);
    return y;
}

int height_difference(ARV* atual){
    int left_height = atual->left == NULL ? -1 : atual->left->height;
    int right_height = atual->right == NULL ? -1 : atual->right->height;
    return right_height - left_height;
}

ARV* helper_arvore_inserir(ARV* atual, ARV* novo, bool *adicionado){
    if (atual == NULL) return novo;
    //rintf("searchin %d\n", atual->valor);
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
    if (diff >= 2){
        if (height_difference(atual->right) < 0){
            atual->right = right_rotation(atual->right);
        }
        atual = left_rotation(atual);
    }
    else if (diff <= -2){
        if (height_difference(atual->left) > 0){
            atual->left = left_rotation(atual->left);
        }
        atual = right_rotation(atual);
    }
    return atual;
}

bool arvore_inserir(AVL* avl, int item){
    bool adicionado = true;
    avl->arvore = helper_arvore_inserir(avl->arvore, criar_no(item), &adicionado);
    avl->size += adicionado;
    return adicionado;
}

ARV* find_min(ARV *arv){
    return (arv->left == NULL ? arv : find_min(arv->left));
}

void recalcular_altura(ARV *arv){
    if (arv->left == arv->right) arv->height = 0;
    else if (arv->left == NULL) arv->height = arv->right->height + 1;
    else if (arv->right == NULL) arv->height = arv->left->height + 1;
    else arv->height = max(arv->left->height, arv->right->height) + 1;
}

ARV *helper_arvore_remover(ARV* avl, int item, bool *removido){
    if (avl == NULL) return NULL;
    if (avl->valor == item){
        *removido = true;
        if (avl->left == avl->right){
            free(avl);
            return NULL;
        }
        else if(avl->left == NULL || avl->right == NULL){
            ARV *tmp = (avl->left == NULL) ? avl->right : avl->left;
            free(avl);
            return tmp;
        }
        else{
            int tmp = find_min(avl->right)->valor;
            avl->right = helper_arvore_remover(avl->right, tmp, removido);
            recalcular_altura(avl);
            avl->valor = tmp;
        }
    }
    else if (avl->valor > item){ 
        avl->left = helper_arvore_remover(avl->left, item, removido);
    }
    else {
        avl->right = helper_arvore_remover(avl->right, item, removido);
    }
    recalcular_altura(avl);
    if (height_difference(avl) >= 2){
        if (height_difference(avl->right) < 0){
            avl->right = right_rotation(avl->right);
        }
        avl = left_rotation(avl);
    }
    else if (height_difference(avl) <= -2){
        if (height_difference(avl->left) > 0){
            avl->left = left_rotation(avl->left);
        }
        avl = right_rotation(avl);
    }
    return avl;
}

bool arvore_remover(AVL *arv, int item){
    bool removido = false;
    arv->arvore = helper_arvore_remover(arv->arvore, item, &removido);
    arv->size -= removido;
    return removido;
}

void helper_arvore_apagar(ARV *arv){
    if (arv == NULL) return;
    helper_arvore_apagar(arv->left);
    helper_arvore_apagar(arv->right);
    free(arv);
}

void arvore_apagar(AVL *arv){
    helper_arvore_apagar(arv->arvore);
    free(arv);
}

void helper_arvore_imprimir(ARV *arv){
    if (!arv) return;
    helper_arvore_imprimir(arv->left);
    printf("%d ", arv->valor);
    helper_arvore_imprimir(arv->right);
}

void arvore_imprimir(AVL *avl){
    helper_arvore_imprimir(avl->arvore);
}

bool pertence_helper(ARV *arv, int valor){
    if (arv == NULL) return 0;
    if (arv->valor == valor) return 1;
    if (arv->valor > valor) return pertence_helper(arv->left, valor);
    else return pertence_helper(arv->right, valor);
}

bool arvore_pertence(AVL *avl, int valor){
    return pertence_helper(avl->arvore, valor);
}

bool arvore_get_tamanho(AVL *avl){
    return avl->size;
}

void insert_tree(AVL *nova, ARV *original){
    if (original == NULL || nova == NULL) return;
    arvore_inserir(nova, original->valor);
    insert_tree(nova, original->left);
    insert_tree(nova, original->right);
}

AVL *arvore_merge(AVL* uniao, AVL *a, AVL *b){
    insert_tree(uniao, a->arvore);
    insert_tree(uniao, b->arvore);
    return uniao;
}