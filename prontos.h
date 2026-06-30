#ifndef PRONTOS_H
#define PRONTOS_H

#include "constants.h"

// Inicializa uma fila de processos prontos com uma prioridade específica
void inicializaFilaPronto(FilaProntos* F, Prioridade P);

// Adiciona um processo à fila de prontos
void novoPronto(FilaProntos* F, PCB* pcb);

// Remove e retorna o primeiro processo de uma fila de prontos
PCB* pop(FilaProntos* F);

// Escolhe o próximo processo a ser executado entre filas de alta e baixa prioridade
PCB* escolheProximo(FilaProntos* A, FilaProntos* B);

#endif // PRONTOS_H
