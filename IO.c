#include <stdio.h>
#include "constants.h"
#include "prontos.c"

void inicializaFilaIO(FilaIO* F) {
    F->fim = NULL;
    F->inicio = NULL;
    F->tamanho = 0;
}

// recebe a lista de filas, um processo, e um tipo de bloqueio, 
// e adiciona esse processo na fila de bloqueados para aquele dispositivo
void bloqueioIO(FilaIO filas[3], PCB* pcb, TipoIO tipo) {
    pcb->status = BLOQUEADO;

    ProcessoIO *novo = malloc(sizeof(ProcessoIO)); // cria e inicializa o processo na fila
    novo->pcb = *pcb;
    novo->proximo = NULL;
    novo->espera = tipo;
    free(pcb);

    FilaIO* F; // escolhe para qual das filas vai adicionar o processo baseado no tipo
    switch (tipo)
        {
        case DISCO:
        FilaIO* F = &filas[0];
        break;
        case FITA:
        FilaIO* F = &filas[1];
        break;
        case IMPRESSORA:
        FilaIO* F = &filas[2];
        break;
    default:
        break;
    }

    if (F->tamanho == 0) { // se a fila estiver vazia, coloca na frente e no final, se não, coloca no final.
        F->fim = novo;
        F->inicio = novo;
    } else {
        F->fim->proximo = novo;
        F->fim = novo;
    }
    F->tamanho++;
}


// verifica se o tempo de IO acabou em uma das 3 filas, se sim, envia o processo para a fila de prontos respectiva.
void gerenciaFilaIO(FilaIO IO[3], FilaProntos* fila_alta, FilaProntos* fila_baixa) {
    
    for (int i = 0; i<3; i++) {
        if( IO[i].inicio == NULL) continue;

        IO[i].inicio->espera--;
        if (IO[i].inicio->espera == 0) {
            ProcessoIO* primeiro = IO[i].inicio;
            primeiro->tipo == DISCO ? novoPronto(fila_baixa, primeiro) : novoPronto(fila_alta, primeiro);
            IO[i].inicio->proximo = IO[i].inicio;
            if (IO[i].inicio == NULL) IO[i].fim = NULL; 
            free(primeiro);
            IO[i].tamanho--;
        }
    }
}
