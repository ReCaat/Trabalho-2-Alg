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

void pre(NODE* raiz);
void pos_delete(NODE** root);
NODE* left_rotation(NODE* x);
NODE* right_rotation(NODE* x);
NODE* rec_inserir(NODE *raiz, NODE* Novo, bool* confirma);
NODE* rec_remove(NODE** raiz, int chave, bool* sucesso);
NODE* remove_min(NODE** raiz);

//Inicio de funções de usuário
ARN *arn_criar (void) {
    ARN *NewT = (ARN*) malloc(sizeof(ARN));

    if(NewT != NULL) {
        NewT->raiz = NULL;
    } else
        printf("Erro ao criar. Espaço insuficiente");

    return NewT;
}

bool arn_inserir (ARN *T, int chave) {
    if(T == NULL){printf("Erro ao inserir. Árvore não existe"); return false;}

    //Se a árvore não for nula ele cria o nó
    NODE *NewN = (NODE*) malloc(sizeof(NODE));

    if(NewN == NULL) {
        printf("Erro ao inserir. Espaço insuficiente");
        return false; //Se a alocação do novo nó for má sucedida
    } else {
        NewN->Esquerda = NULL;
        NewN->Direita = NULL;
        NewN->chave = chave;
        NewN->cor = RED;
    }

    bool confirma = true;
    //caso o número a ser inserido já estiver na árvore confirma = false
    T->raiz = rec_inserir(T->raiz, NewN, &confirma);
    T->raiz->cor = BLACK; //Ajuste do primeiro caso

    return confirma;
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
    bool sucesso = false;

    T->raiz = rec_remove(&T->raiz, chave, &sucesso);
    T->raiz->cor = BLACK;
    return sucesso;    
}

void arn_apagar (ARN **T) {
    if((*T) != NULL) {
        if((*T)->raiz != NULL)
        pos_delete(&(*T)->raiz);

        free(*T);
        *T = NULL;
    }
}

bool arn_pertence(ARN *T, int chave) {
    if(T != NULL) {
        NODE* aux = T->raiz;
        while(aux != NULL) {
            if(chave == aux->chave) return true;

            if(chave < aux->chave) aux = aux->Esquerda;
            else aux = aux->Direita;
        }
    }
    return false;
}

bool arn_vazia(ARN *T) {
    return T == NULL || T->raiz == NULL;
}
//Fim das funções de usuário  

//Auxilia imprimir a árvore
void pre(NODE* raiz) {
    if(raiz != NULL) {
        printf("%d ", raiz->chave);
        if(raiz->cor) printf("RED"); else printf("BLACK");
        printf("\n");
        pre(raiz->Esquerda);
        pre(raiz->Direita);
    }
}

//Auxilia apagar a árvore
void pos_delete(NODE** root) {
    if(root != NULL) {
        pos_delete(&(*root)->Esquerda);
        pos_delete(&(*root)->Direita);
        free(*root);
        *root = NULL;
    }
}

//Funções auxiliares para remoção e deleção
bool eh_vermelho(NODE* raiz) {
    if(raiz == NULL) return false;
    return raiz->cor == RED;
}

NODE* left_rotation(NODE* x){
    NODE* y = x->Direita;
    x->Direita = y->Esquerda;
    y->Esquerda = x;
    
    int aux = x->cor;
    x->cor = y->cor;
    y->cor = aux;

    return y;
}

NODE* right_rotation(NODE* x){
    NODE* y = x->Esquerda;
    x->Esquerda = y->Direita;
    y->Direita = x;

    int aux = x->cor;
    x->cor = y->cor;
    y->cor = aux;

    return y;
}

void inverte_cores(NODE* x) {
    x->cor = !x->cor;
    x->Direita->cor = !x->Direita->cor;
    x->Esquerda->cor = !x->Esquerda->cor;
}

NODE* acha_min(NODE* trocado) {
    NODE* aux = trocado;
    while(aux->Esquerda != NULL)
        aux = aux->Esquerda;
    
    return aux;
}

//true - esquerda
//false - direita
NODE* move_red(NODE* raiz, bool lado) {

    inverte_cores(raiz); 
    if(lado && eh_vermelho(raiz->Direita->Esquerda)) {
        raiz->Direita = right_rotation(raiz->Direita);
        raiz = left_rotation(raiz);
        inverte_cores(raiz);
    } 
    
    if(!lado && eh_vermelho(raiz->Esquerda->Esquerda)) {
        raiz = right_rotation(raiz);
        inverte_cores(raiz);
    }

    return raiz;
}

//Na volta da recursão reorganiza a árvore
NODE* arruma(NODE* raiz) {
    if(raiz != NULL) {
        if(eh_vermelho(raiz->Direita) && !eh_vermelho(raiz->Esquerda)) raiz = left_rotation(raiz);

        if(eh_vermelho(raiz->Esquerda) && eh_vermelho(raiz->Esquerda->Esquerda)) raiz = right_rotation(raiz);
        
        if(eh_vermelho(raiz->Direita) && eh_vermelho(raiz->Esquerda)) inverte_cores(raiz);
    }

    return raiz;
}

//Principal inserção
NODE* rec_inserir(NODE *raiz, NODE* Novo, bool* confirma) {
    if(raiz == NULL)
        return Novo;

    if(raiz->chave == Novo->chave) {
        *confirma = false;
        free(Novo); Novo = NULL;
        return raiz; //Evita valores duplicados
    }
    
    if(Novo->chave < raiz->chave)
        raiz->Esquerda = rec_inserir(raiz->Esquerda, Novo, confirma);
    else
        raiz->Direita = rec_inserir(raiz->Direita, Novo, confirma);
    
    return arruma(raiz);
}

//Deleção principal
NODE* remove_min(NODE** raiz) {
    if(*raiz == NULL) return NULL;

    if((*raiz)->Esquerda == NULL) {
        //Não precisa se preocupar se existe um nó a direita pelas propriedades da árvore
        free(*raiz); *raiz = NULL; 
        return NULL;
    }

    if(!eh_vermelho((*raiz)->Esquerda) && !eh_vermelho((*raiz)->Esquerda->Esquerda))
        *raiz = move_red(*raiz, true);

    (*raiz)->Esquerda = remove_min(&(*raiz)->Esquerda);

    return arruma(*raiz);
}

NODE* rec_remove(NODE** raiz, int chave, bool *sucesso) {
    if(*raiz == NULL) return NULL;

    if(chave < (*raiz)->chave) {
        if((*raiz)->Esquerda != NULL && !eh_vermelho((*raiz)->Esquerda) && !eh_vermelho((*raiz)->Esquerda->Esquerda))
            *raiz = move_red(*raiz, true);

        (*raiz)->Esquerda = rec_remove(&(*raiz)->Esquerda, chave, sucesso);
    } else {

        if(eh_vermelho((*raiz)->Esquerda))
            *raiz = right_rotation(*raiz);

        if(chave == (*raiz)->chave && (*raiz)->Direita == NULL) {
            *sucesso = true;
            free(*raiz); *raiz = NULL; 
            return NULL;
        }

        if((*raiz)->Direita != NULL && !eh_vermelho((*raiz)->Direita) && !eh_vermelho((*raiz)->Direita->Esquerda))
            *raiz = move_red(*raiz, false);

        if(chave == (*raiz)->chave) {
            *sucesso = true;
            (*raiz)->chave = acha_min((*raiz)->Direita)->chave;
            (*raiz)->Direita = remove_min(&(*raiz)->Direita);
        } 
        else  (*raiz)->Direita = rec_remove(&(*raiz)->Direita, chave, sucesso);
    }

    return arruma(*raiz);
}

void rec_uniao(ARN *nova, NODE *original){
    if (original == NULL || nova == NULL) return;
    arn_inserir(nova, original->chave);
    rec_uniao(nova, original->Esquerda);
    rec_uniao(nova, original->Direita);
}

ARN *arn_uniao(ARN* uniao, ARN *a, ARN *b){
    rec_uniao(uniao, a->raiz);
    rec_uniao(uniao, b->raiz);
    return uniao;
}

void rec_intersect(ARN *interseccao, ARN *b, NODE *no_de_a){
    if (interseccao == NULL || no_de_a == NULL) return;
    if (arn_pertence(b, no_de_a->chave)) arn_inserir(interseccao, no_de_a->chave);
    rec_intersect(interseccao, b, no_de_a->Esquerda);
    rec_intersect(interseccao, b, no_de_a->Direita);
}

void arn_intersect(ARN* intersection, ARN *a, ARN *b){
    
    if(a == NULL || b == NULL) return;
    rec_intersect(intersection, b, a->raiz);
}
