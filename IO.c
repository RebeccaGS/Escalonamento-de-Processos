#include <stdio.h>
#include "constants.h"
#include "prontos.c"

void inicializaFila(FilaIO* F) {
    F->fim = NULL;
    F->inicio = NULL;
    F->tamanho = 0;
}

// recebe uma fila, um processo, e um tipo de bloqueio, 
// e adiciona esse processo na fila de bloqueados para aquele dispositivo
void bloqueioIO(FilaIO* F, PCB* pcb, TipoIO tipo) {
    pcb->status = BLOQUEADO;

    ProcessoIO *novo; // cria e inicializa o processo na fila
    novo->pcb = *pcb;
    novo->proximo = NULL;
    novo->espera = tipo;

    if (F->tamanho == 0) { // se a fila estiver vazia, coloca na frente e no final, se não, coloca no final.
        F->fim = novo;
        F->inicio = novo;
    } else {
        F->fim->proximo = novo;
        F->fim = novo;
    }
    F->tamanho++;
}


// verifica se o tempo de IO acabou, se acabou, muda o primeiro processo pra fila de prontos
void fimIO(FilaIO* IO, FilaProntos* Prontos) {
    IO->inicio->espera--; // decrementa o contador do primeiro da fila

    if (IO->inicio->espera == 0) {
        PCB *primeiro = IO->inicio;
        IO->inicio->proximo = IO->inicio;
        novoPronto(Prontos, primeiro);
    }
}
