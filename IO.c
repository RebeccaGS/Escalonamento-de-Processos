#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "prontos.h"
#include "IO.h"

void inicializaFilaIO(FilaIO* F) {
    if (F == NULL) {
        return;
    }

    F->fim = NULL;
    F->inicio = NULL;
    F->tamanho = 0;
}

// recebe a lista de filas, um processo, e um tipo de bloqueio,
// e adiciona esse processo na fila de bloqueados para aquele dispositivo
void bloqueioIO(FilaIO filas[3], PCB* pcb, TipoIO tipo) {
    if (pcb == NULL) {
        return;
    }

    if (tipo != DISCO && tipo != FITA && tipo != IMPRESSORA) {
        return;
    }

    pcb->status = BLOQUEADO;

    ProcessoIO *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return;
    }

    novo->pcb = *pcb;
    novo->proximo = NULL;
    novo->tipo = tipo;
    novo->espera = tipo;

    FilaIO* F = NULL;
    switch (tipo) {
        case DISCO:
            F = &filas[0];
            break;
        case FITA:
            F = &filas[1];
            break;
        case IMPRESSORA:
            F = &filas[2];
            break;
        default:
            free(novo);
            return;
    }

    if (F == NULL) {
        free(novo);
        return;
    }

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

// verifica se o tempo de IO acabou em uma das 3 filas, se sim, envia o processo para a fila de prontos respectiva.
void gerenciaFilaIO(FilaIO IO[3], FilaProntos* fila_alta, FilaProntos* fila_baixa) {
    if (fila_alta == NULL || fila_baixa == NULL) {
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (IO[i].inicio == NULL) {
            continue;
        }

        ProcessoIO* primeiro = IO[i].inicio;
        if (primeiro->espera > 0) {
            primeiro->espera--;
        }

        if (primeiro->espera == 0) {
            PCB* pcb = malloc(sizeof(*pcb));
            if (pcb == NULL) {
                continue;
            }

            *pcb = primeiro->pcb;
            primeiro->tipo == DISCO ? novoPronto(fila_baixa, pcb) : novoPronto(fila_alta, pcb);

            IO[i].inicio = primeiro->proximo;
            if (IO[i].inicio == NULL) {
                IO[i].fim = NULL;
            }
            free(primeiro);
            IO[i].tamanho--;
        }
    }
}
