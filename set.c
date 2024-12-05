#include "set.h"
//esse programa serve puramente para criar uma uniao das interfaces da AVL e rubo-negro a fim de simplificar as interações do usuário
//Quando o tipo eh 1, usa-se a interface arn.h e quando é 0 usa-se da avl.h
struct set{
    int tipo;
    void *estrutura;
};

SET *set_criar(unsigned char tipo){
    SET *novo_set = malloc(sizeof(SET));
    novo_set->tipo = tipo;
    if (tipo == 0){
        novo_set->estrutura = arvore_criar();
    }
    else {
        novo_set->estrutura = arn_criar();
    }
    return novo_set;
}
bool set_pertence(SET *A, int elemento){
    bool answ = false;
    if (A->tipo == 0){
        answ = arvore_pertence(A->estrutura, elemento);
    }
    else{
        answ = arn_pertence(A->estrutura, elemento);
    }
    return answ;
}
bool set_inserir (SET *s, int elemento){
    bool success = false;
    if (s->tipo == 0){
        success = arvore_inserir(s->estrutura, elemento);
    }
    else{
        success = arn_inserir(s->estrutura, elemento);
    }
    return success;
}
bool set_remover(SET *s, int elemento){
    bool success = false;
    if (s->tipo == 0){
        success = arvore_remover(s->estrutura, elemento);
    }
    else{
        success = arn_remover(s->estrutura, elemento);
    }
    return success;
}
void set_apagar(SET **s){
    if ((*s)->tipo == 0){
        arvore_apagar((*s)->estrutura);
    }
    else {
        arn_apagar((*s)->estrutura);
    }
    free(*s);
    *s = NULL;
}
void set_imprimir(SET *s){
    if (s->tipo == 0){
        arvore_imprimir(s->estrutura);
    }
    else{
        arn_imprimir(s->estrutura);
    }
}
SET *set_uniao(SET *A, SET *B){
    SET *novo_set = set_criar(A->tipo);
    if (A->tipo == 0){
        arvore_merge(novo_set->estrutura, A->estrutura, B->estrutura);
    }
    else{
        arn_uniao(novo_set->estrutura, A->estrutura, B->estrutura);
    }
    return novo_set;
}
SET *set_interseccao(SET *A, SET *B){
    SET *interseccao = set_criar(A->tipo);
    if (A->tipo == 1){
        arvore_intersect(interseccao->estrutura, A->estrutura, B->estrutura);
    }
    else{
        arn_intersect(interseccao->estrutura, A->estrutura, B->estrutura);
    }
    return interseccao;
}
