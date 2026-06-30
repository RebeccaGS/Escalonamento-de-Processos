#ifndef PROCESSOS_H
#define PROCESSOS_H

#include "constants.h"

// Cria um novo processo com PID e inicializa suas propriedades
PCB* inicializaProcesso(int pid);

// Inicializa uma fila de processos vazia
void inicializaFila(FilaProcessos* F);

// Adiciona um novo processo ao final da fila
void novoProcesso(FilaProcessos *fila, PCB* pcb);

// Lê a tabela.txt e carrega os processos numa fila
void lerLista(FilaProcessos* fila);

// Remove e retorna o primeiro processo da fila
PCB* popProcesso(FilaProcessos* F);

#endif // PROCESSOS_H
