#include <stdio.h>
#include "constants.h"
#include "prontos.c"

void inicializaFilaIO(FilaIO* F) {
    F->fim = NULL;
    F->inicio = NULL;
    F->tamanho = 0;
}

// recebe uma fila, um processo, e um tipo de bloqueio, 
// e adiciona esse processo na fila de bloqueados para aquele dispositivo
void bloqueioIO(FilaIO* F, PCB* pcb, TipoIO tipo) {
    pcb->status = BLOQUEADO;

    ProcessoIO *novo = malloc(sizeof(ProcessoIO)); // cria e inicializa o processo na fila
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
void gerenciaFilaIO(FilaIO* IO, FilaProntos* fila_alta, FilaProntos* fila_baixa) {
    if( IO-> inicio == NULL) return;

    ProcessoIO* ocupando_atual = IO ->inicio;
    ocupando_atual->espera--;

    if(ocupando_atual->espera <= 0){
        //TODO: VER SE VAI QUERER PRINTF
        
        if(ocupando_atual->tipo == DISCO){
            novoPronto(fila_baixa, &(ocupando_atual->pcb));
        } else {
            novoPronto(fila_alta, &(ocupando_atual->pcb));
        }

        IO->inicio = ocupando_atual->proximo;
        if (IO->inicio == NULL) IO->fim = NULL;

        free(ocupando_atual);
        IO->tamanho--;
    }

}
