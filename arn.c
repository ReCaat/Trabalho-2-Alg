#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arn.h"

typedef struct node_ {
    struct node_ *Esquerda;
    struct node_ *Direita;
    int chave;
    int cor;
} NODE;

struct arn_ {
    NODE *raiz;
};

void pre(NODE* Raiz);
void pos_delete(NODE* root);
NODE* criar_node(int chave);
NODE* left_rotation(NODE* x);
NODE* right_rotation(NODE* x);
//NODE* criar_node(int chave);
NODE* rec_inserir(NODE *Raiz, NODE* Novo);
NODE* rec_remove(NODE* Raiz, int chave);
NODE* remove_min(NODE* Raiz);

ARN *arn_criar (void) {
    ARN *NewT = (ARN*) malloc(sizeof(ARN));

    if(NewT != NULL)  
        NewT->raiz = NULL;
    else
        printf("Erro ao criar. Espaço insuficiente");

    return NewT;
}

bool arn_inserir (ARN *T, int chave) {
    if(T == NULL){printf("Erro ao inserir. Árvore não existe"); return false;}

    NODE *NewN = (NODE*) malloc(sizeof(NODE));

    if(NewN == NULL) 
        printf("Erro ao inserir. Espaço insuficiente");
    else {
        NewN->Esquerda = NULL;
        NewN->Direita = NULL;
        NewN->chave = chave;
        NewN->cor = RED;
    }

    T->raiz = rec_inserir(T->raiz, NewN);
    T->raiz->cor = BLACK;

    return true;
}

void arn_imprimir (ARN *T) {
    if(T != NULL) {
        pre(T->raiz);
        printf("\n");
    } else 
        printf("Erro ao imprimir, a árvore não existe");
}

bool arn_remover(ARN *T, int chave) {
    if(T == NULL){printf("Erro ao remover. Árvore não existe."); return false;}

    T->raiz = rec_remove(T->raiz, chave);
    T->raiz->cor = BLACK;
    return true;    
}


void arn_apagar (ARN **T) {
    if((*T) != NULL) {
        if((*T)->raiz != NULL)
        pos_delete((*T)->raiz);

        free(*T);
        *T = NULL;
    }
}

int *arn_busca(ARN *T, int chave) {
    if(T != NULL) {
        NODE* aux = T->raiz;
        while(aux != NULL) {
            if(chave == aux->chave) return &(aux->chave);

            if(chave < aux->chave) aux = aux->Esquerda;
            else aux = aux->Direita;
        }
    }
    return NULL;
}

bool arn_vazia(ARN *T) {
    return T == NULL || T->raiz == NULL;
}
  
void pre(NODE* Raiz) {
    if(Raiz != NULL) {
        printf("%d ", Raiz->chave);
        if(Raiz->cor) printf("RED"); else printf("BLACK");
        printf("\n");
        pre(Raiz->Esquerda);
        pre(Raiz->Direita);
    }
}

void pos_delete(NODE* root) {
    if(root != NULL) {
        pos_delete(root->Esquerda);
        pos_delete(root->Direita);
        free(root);
    }
}

bool eh_vermelho(NODE* Raiz) {
    if(Raiz == NULL) return false;
    return Raiz->cor == RED;
}

NODE* left_rotation(NODE* x){
    printf("esquerda\n");
    NODE* y = x->Direita;
    x->Direita = y->Esquerda;
    y->Esquerda = x;
    
    return y;
}

NODE* right_rotation(NODE* x){
    printf("direita\n");
    NODE* y = x->Esquerda;
    x->Esquerda = y->Direita;
    y->Direita = x;

    return y;
}

void swap_colors(NODE* x, NODE* y) {
    int aux = x->cor;
    x->cor = y->cor;
    y->cor = aux;
}

NODE* rec_inserir(NODE *Raiz, NODE* Novo) {
    if(Raiz == NULL)
        Raiz = Novo;
    else if(Novo->chave < Raiz->chave)
        Raiz->Esquerda = rec_inserir(Raiz->Esquerda, Novo);
    else
        Raiz->Direita = rec_inserir(Raiz->Direita, Novo);
    

    if(eh_vermelho(Raiz->Direita) && !eh_vermelho(Raiz->Esquerda)) {
        Raiz = left_rotation(Raiz);
        swap_colors(Raiz, Raiz->Esquerda);
    }

    if(eh_vermelho(Raiz->Esquerda) && eh_vermelho(Raiz->Esquerda->Esquerda)) {
        Raiz = right_rotation(Raiz);
        swap_colors(Raiz, Raiz->Direita);
    }

    if(eh_vermelho(Raiz->Direita) && eh_vermelho(Raiz->Esquerda)) {
        Raiz->cor = !Raiz->cor;
        Raiz->Direita->cor = BLACK;
        Raiz->Esquerda->cor = BLACK;
    }

    return Raiz;
}

NODE* acha_min(NODE* trocado) {
    NODE* aux = trocado;
    while(aux->Esquerda != NULL)
        aux = aux->Esquerda;
    
    return aux;
}

NODE* move_esquerda(NODE* Raiz) {
    Raiz->cor = BLACK;
    Raiz->Esquerda->cor = RED; 
    if(eh_vermelho(Raiz->Direita->Esquerda)) {
        Raiz->Direita = right_rotation(Raiz->Direita);
        Raiz = left_rotation(Raiz);
    } else 
        Raiz->Direita->cor = RED;
    return Raiz;
}

NODE* move_direita(NODE* Raiz) {
    Raiz->cor = BLACK;
    Raiz->Direita->cor = RED; 
    if(Raiz->Esquerda != NULL && eh_vermelho(Raiz->Esquerda->Esquerda)) {
        Raiz = right_rotation(Raiz);
        Raiz->cor = RED;
        Raiz->Esquerda->cor = BLACK;
    } else 
        Raiz->Esquerda->cor = RED;
    return Raiz;
}

NODE* remove_min(NODE* Raiz) {
    if(Raiz->Esquerda == NULL) {
        NODE* aux = Raiz->Direita;
        free(Raiz); Raiz = NULL; 
        return aux;
    }

    if(!eh_vermelho(Raiz->Esquerda) && !eh_vermelho(Raiz->Esquerda->Esquerda))
        Raiz = move_esquerda(Raiz);

    Raiz->Esquerda = remove_min(Raiz->Esquerda);

    if(eh_vermelho(Raiz->Direita)) {
        Raiz = left_rotation(Raiz);
        swap_colors(Raiz, Raiz->Esquerda);
    }

    if(eh_vermelho(Raiz->Esquerda) && eh_vermelho(Raiz->Esquerda->Esquerda)) {
        Raiz = right_rotation(Raiz);
        Raiz->Esquerda->cor = BLACK;
    }

    return Raiz;
}

NODE* rec_remove(NODE* Raiz, int chave) {

    if(chave < Raiz->chave) {
        if(!eh_vermelho(Raiz->Esquerda) && !eh_vermelho(Raiz->Esquerda->Esquerda))
            Raiz = move_esquerda(Raiz);

        Raiz->Esquerda = rec_remove(Raiz->Esquerda, chave);
    } else {

        if(eh_vermelho(Raiz->Esquerda)){
            Raiz = right_rotation(Raiz);
            swap_colors(Raiz, Raiz->Direita);    
        }

        if(chave == Raiz->chave && Raiz->Direita == NULL) 
            return NULL;

        if(!eh_vermelho(Raiz->Direita) && !eh_vermelho(Raiz->Direita->Esquerda))
            Raiz = move_direita(Raiz);

        if(chave == Raiz->chave) {
            Raiz->chave = acha_min(Raiz->Direita)->chave;
            Raiz->Direita = remove_min(Raiz->Direita);
        } 
        else  Raiz->Direita = rec_remove(Raiz->Direita, chave);
    }
    
    if(eh_vermelho(Raiz->Direita)) Raiz = left_rotation(Raiz);

    if(eh_vermelho(Raiz->Esquerda) && eh_vermelho(Raiz->Esquerda->Esquerda)) {
        Raiz = right_rotation(Raiz);
        Raiz->Esquerda->cor = BLACK;
    }

    return Raiz;
}