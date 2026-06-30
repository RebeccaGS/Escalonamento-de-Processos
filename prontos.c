#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

// inicializa uma fila de uma prioridade alta ou baixa.
void inicializaFilaPronto(FilaProntos* F, Prioridade P) {
    if (F == NULL) {
        return;
    }

    F->fim = NULL;
    F->inicio = NULL;
    F->prioridade = P;
    F->tamanho = 0;
}

// recebe uma fila, e um PCB, e coloca esse processo no final da fila
void novoPronto(FilaProntos* F, PCB* pcb) {
    if (F == NULL || pcb == NULL) {
        return;
    }

    pcb->status = PRONTO;
    pcb->prioridade = F->prioridade;

    ProcessoPronto *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return;
    }

    novo->pcb = *pcb;
    novo->proximo = NULL;

    if (F->tamanho == 0) {
        F->fim = novo;
        F->inicio = novo;
    } else {
        F->fim->proximo = novo;
        F->fim = novo;
    }

    F->tamanho++;
    free(pcb);
}

PCB* pop(FilaProntos* F) {
    if (F == NULL || F->inicio == NULL) {
        return NULL;
    }

    ProcessoPronto *primeiro = F->inicio;
    PCB *pcb = malloc(sizeof(*pcb));
    if (pcb == NULL) {
        return NULL;
    }

    *pcb = primeiro->pcb;

    F->inicio = primeiro->proximo;
    if (F->inicio == NULL) {
        F->fim = NULL;
    }
    F->tamanho--;
    free(primeiro);

    return pcb;
}

PCB* escolheProximo(FilaProntos* A, FilaProntos* B) {
    if (A != NULL && A->tamanho != 0) {
        return pop(A);
    }

    if (B != NULL && B->tamanho != 0) {
        return pop(B);
    }

    return NULL;
}