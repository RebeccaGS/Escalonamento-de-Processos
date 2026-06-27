#include <stdio.h>
#include "constants.h"

// inicializa uma fila de uma prioridade alta ou baixa.
void inicializaFila(FilaProntos* F, Prioridade P) {
    F->fim = NULL;
    F->inicio = NULL;
    F->prioridade = P;
    F->tamanho = 0;
}

// recebe uma fila, e um PCB, e coloca esse processo no final da fila
void novoPronto(FilaProntos* F, PCB* pcb) {
    pcb->status = PRONTO;
    pcb->prioridade = F->prioridade;

    ProcessoPronto *novo; // cria e inicializa o processo na fila
    novo->pcb = *pcb;
    novo->proximo = NULL;

    if (F->tamanho == NULL) { // se a fila estiver vazia, coloca na frente e no final, se não, coloca no final.
        F->fim = novo;
        F->inicio = novo;
    } else {
        F->fim->proximo = novo;
        F->fim = novo;
    }
    F->tamanho++;
}

PCB* pop(FilaProntos* F) {
    PCB *primeiro = F->inicio;
    F->inicio->proximo = F->inicio;
    return primeiro;
}
