/*
Código correspondente a implementação do conjunto 2, ele foi escolhido como sendo a árvore rubro-negro (variação left-leaning).
Essa escolha foi feita graças aos seus tempos de operações demoradas:
Inserção: O(logn);
Remoção: O(logn);
Busca/Pertencimento: O(logn);
Isso ocorre pois ela tenta se manter balanceada através de suas regras descritas no relatório.

Autoria: 
João Victor Alonso de Mello / N°USP - 10951790
Renan Banci Catarin / N°USP - 14658181
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arn.h"

typedef struct node_ { //Declaração de uma estrutura de nó.
    struct node_ *Esquerda;
    struct node_ *Direita;
    int chave;
    int cor; //a cor representa a da aresta que "chega" no nó.
} NODE;

struct arn_ {
    NODE *raiz;
};

//Declaração prévias de funções de suporte para aquelas disponíveis ao usuário -- explicadas posteriormente.
void pre(NODE* raiz);
void pos_delete(NODE** root);
NODE* rotacao_esq(NODE* x);
NODE* rotacao_dir(NODE* x);
NODE* rec_inserir(NODE *raiz, NODE* Novo, bool* confirma);
NODE* rec_remove(NODE** raiz, int chave, bool* sucesso);
NODE* remove_min(NODE** raiz);
void rec_uniao(ARN *nova, NODE *original);
void rec_intersect(ARN *interseccao, ARN *b, NODE *no_de_a);

//Inicio de funções de usuário -- já comentados no arn.h.
ARN *arn_criar (void) {
    ARN *NewT = (ARN*) malloc(sizeof(ARN));

    if(NewT != NULL) {
        NewT->raiz = NULL;
    } else
        printf("Erro ao criar. Espaço insuficiente");//tratamento de erro.

    return NewT;
}

bool arn_inserir (ARN *T, int chave) {
    if(T == NULL){printf("Erro ao inserir. Árvore não existe"); return false;} //tratamento de erro.

    //Se a árvore não for nula ele cria o nó.
    NODE *NewN = (NODE*) malloc(sizeof(NODE));

    if(NewN == NULL) {
        printf("Erro ao inserir. Espaço insuficiente");
        return false; //Se a alocação do novo nó for má sucedida.
    } else {
        NewN->Esquerda = NULL; 
        NewN->Direita = NULL;
        NewN->chave = chave;
        NewN->cor = RED; //Inicializa o novo nó como vermelho e com filhos nulos.
    }

    bool confirma = true;
    //caso o número a ser inserido já estiver na árvore confirma = false.
    T->raiz = rec_inserir(T->raiz, NewN, &confirma);
    T->raiz->cor = BLACK; //Ajuste do primeiro caso.

    return confirma;
}

bool arn_remover(ARN *T, int chave) {
    if(T == NULL){printf("Erro ao remover. Árvore não existe."); return false;} //tratamento de erro.
    bool sucesso = false;

    if(T->raiz != NULL) T->raiz = rec_remove(&T->raiz, chave, &sucesso);
    if(T->raiz != NULL) T->raiz->cor = BLACK;
    return sucesso;    
}

bool arn_pertence(ARN *T, int chave) {
    //feito iterativamente por simplicidade.
    if(T != NULL) { //tratamento de erro.
        NODE* aux = T->raiz;
        while(aux != NULL) {
            if(chave == aux->chave) return true;

            if(chave < aux->chave) aux = aux->Esquerda;
            else aux = aux->Direita;
        }
    }
    return false;
}

void arn_imprimir (ARN *T) {
    if(T != NULL) { //tratamento de erro.
        pre(T->raiz);
        printf("\n");
    } else 
        printf("Erro ao imprimir, a árvore não existe");
}

void arn_apagar (ARN *T) {
    if(T != NULL) { //tratamento de erro.
        pos_delete(&T->raiz);

        free(T);
        T = NULL;
    }
}

bool arn_vazia(ARN *T) {
    return T == NULL || T->raiz == NULL;
}

void arn_uniao(ARN* uniao, ARN *A, ARN *B){
    rec_uniao(uniao, A->raiz);
    rec_uniao(uniao, B->raiz);
}

void arn_intersect(ARN* intersection, ARN *A, ARN *B){
    
    if(A == NULL || B == NULL) return;
    rec_intersect(intersection, B, A->raiz);
}
//Fim das funções de usuário.

//Auxilia imprimir a árvore em ordem.
void pre(NODE* raiz) {
    if(raiz != NULL) {
        pre(raiz->Esquerda);
        printf("%d, ", raiz->chave);
        pre(raiz->Direita);
    }
}

//Auxilia apagar a árvore em pós-ordem.
void pos_delete(NODE** root) {
    if(*root != NULL) {
        pos_delete(&(*root)->Esquerda);
        pos_delete(&(*root)->Direita);
        free(*root);
        *root = NULL;
    }
}

//Funções auxiliares para remoção e deleção.
//Detecta se um dado nó é vermelho ou negro, lembrando que pelas regras da árvore nós nulos são negros.
bool eh_vermelho(NODE* raiz) {
    if(raiz == NULL) return false;
    return raiz->cor == RED;
}

//Rotaciona um nó para a esquerda e troca as cores dele com seu filho da direita, para adequar com a propriedade das arestas.
NODE* rotacao_esq(NODE* x){
    NODE* y = x->Direita;
    x->Direita = y->Esquerda;
    y->Esquerda = x;
    
    int aux = x->cor;
    x->cor = y->cor;
    y->cor = aux;

    return y;
}

//Rotaciona um nó para a direita e troca as cores dele com seu filho esquerdo, para adequar com a propriedade das arestas.
NODE* rotacao_dir(NODE* x){
    NODE* y = x->Esquerda;
    x->Esquerda = y->Direita;
    y->Direita = x;

    int aux = x->cor;
    x->cor = y->cor;
    y->cor = aux;

    return y;
}

//Inverte a cor do nó pai e de seus filhos (caso existirem).
void inverte_cores(NODE* x) {
    x->cor = !x->cor;
    if(x->Direita != NULL) x->Direita->cor = !x->Direita->cor;
    if(x->Esquerda != NULL) x->Esquerda->cor = !x->Esquerda->cor;
}

//Acha o Nó que contem a chave de menor valor -- útil para trocar com o menor ítem do braço direito. Para remoção.
NODE* acha_min(NODE* trocado) {
    NODE* aux = trocado;
    while(aux->Esquerda != NULL)
        aux = aux->Esquerda;
    
    return aux;
}

//Outra função exclusiva da remoção, ela "paa a vermelhidão" na direção do nó que contenha a chave que o usuário deseja remover:
//true - esquerda;
//false - direita.
NODE* move_red(NODE* raiz, bool lado) {

    inverte_cores(raiz); 
    if(lado && eh_vermelho(raiz->Direita->Esquerda)) {
        raiz->Direita = rotacao_dir(raiz->Direita);
        raiz = rotacao_esq(raiz);
        inverte_cores(raiz);
    } 
    
    if(!lado && eh_vermelho(raiz->Esquerda->Esquerda)) {
        raiz = rotacao_dir(raiz);
        inverte_cores(raiz);
    }

    return raiz;
}

//Na volta da recursão reorganiza a árvore e retorna o nó para devido balanceamento negro da árvore.
NODE* arruma(NODE* raiz) {
    if(raiz != NULL) {
        if(eh_vermelho(raiz->Direita) && !eh_vermelho(raiz->Esquerda)) raiz = rotacao_esq(raiz);

        if(eh_vermelho(raiz->Esquerda) && eh_vermelho(raiz->Esquerda->Esquerda)) raiz = rotacao_dir(raiz);
        
        if(eh_vermelho(raiz->Direita) && eh_vermelho(raiz->Esquerda)) inverte_cores(raiz);
    }

    return raiz;
}

//Principal inserção
NODE* rec_inserir(NODE *raiz, NODE* Novo, bool* confirma) {
    if(raiz == NULL)
        return Novo;

    if(raiz->chave == Novo->chave) { //Caso houver um nó com o mesmo valor a serinserido, ele apaga o novo nó, dando status de falha a inserção.
        *confirma = false; 
        free(Novo); Novo = NULL;
        return raiz; //Evita valores duplicados
    }
    
    if(Novo->chave < raiz->chave)
        raiz->Esquerda = rec_inserir(raiz->Esquerda, Novo, confirma);
    else
        raiz->Direita = rec_inserir(raiz->Direita, Novo, confirma);
    
    return arruma(raiz); //Rebalanceamento.
}

//Deleção principal
NODE* remove_min(NODE** raiz) {
    if(*raiz == NULL) return NULL;

    if((*raiz)->Esquerda == NULL) {
        //Não precisa se preocupar se existe um nó a direita pelas propriedades da árvore.
        free(*raiz); *raiz = NULL; 
        return NULL;
    }

    if(!eh_vermelho((*raiz)->Esquerda) && !eh_vermelho((*raiz)->Esquerda->Esquerda))
        *raiz = move_red(*raiz, true);

    (*raiz)->Esquerda = remove_min(&(*raiz)->Esquerda);

    return arruma(*raiz); //Rebalanceamento.
}

NODE* rec_remove(NODE** raiz, int chave, bool *sucesso) {
    if(*raiz == NULL) return NULL;

    if(chave < (*raiz)->chave) {
        if((*raiz)->Esquerda != NULL && !eh_vermelho((*raiz)->Esquerda) && !eh_vermelho((*raiz)->Esquerda->Esquerda))
            *raiz = move_red(*raiz, true); //Mover vermelhidão.

        (*raiz)->Esquerda = rec_remove(&(*raiz)->Esquerda, chave, sucesso);
    } else {

        if(eh_vermelho((*raiz)->Esquerda)) //Garante que não terá nós na esqueda se não houver na direita e move vermelhidão.
            *raiz = rotacao_dir(*raiz);

        if(chave == (*raiz)->chave && (*raiz)->Direita == NULL) { //Caso o nó foi achado e não haja filhos .
            *sucesso = true; //Redefine status da remoção como bem sucedido.
            free(*raiz); *raiz = NULL; 
            return NULL;
        }

        if((*raiz)->Direita != NULL && !eh_vermelho((*raiz)->Direita) && !eh_vermelho((*raiz)->Direita->Esquerda))
            *raiz = move_red(*raiz, false); //Mover vermelhidão

        if(chave == (*raiz)->chave) { //Caso o nó foi achado e tenha filhos.
            *sucesso = true; //Redefine status da remoção como bem sucedido.
            (*raiz)->chave = acha_min((*raiz)->Direita)->chave;
            (*raiz)->Direita = remove_min(&(*raiz)->Direita);
        } 
        else  (*raiz)->Direita = rec_remove(&(*raiz)->Direita, chave, sucesso);
    }

    return arruma(*raiz); //Rebalanceamento.
}

//Ajuda a operação de unir árvores,
//Apenas tenta inserir todos os nós de uma árvore em uma nova.
void rec_uniao(ARN *nova, NODE *original){
    if (original == NULL || nova == NULL) return; //Parada da recursão.
    arn_inserir(nova, original->chave);
    rec_uniao(nova, original->Esquerda);
    rec_uniao(nova, original->Direita);
}

//Ajuda a operação de intersecção de duas árvores,
//Se o nó da árvore A pertence a árvore B ele insere na árvore intersecção.
void rec_intersect(ARN *interseccao, ARN *b, NODE *no_de_a){
    if (interseccao == NULL || no_de_a == NULL) return; //Parada da recursão.
    if (arn_pertence(b, no_de_a->chave)) arn_inserir(interseccao, no_de_a->chave);
    rec_intersect(interseccao, b, no_de_a->Esquerda);
    rec_intersect(interseccao, b, no_de_a->Direita);
}
