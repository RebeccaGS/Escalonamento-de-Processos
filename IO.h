#ifndef IO_H
#define IO_H

#include "constants.h"

// Inicializa uma fila de IO vazia
void inicializaFilaIO(FilaIO* F);

// Bloqueia um processo adicionando-o à fila de IO do dispositivo especificado
void bloqueioIO(FilaIO filas[3], PCB* pcb, TipoIO tipo);

// Gerencia as filas de IO, movendo processos com IO concluído para fila de prontos
void gerenciaFilaIO(FilaIO IO[3], FilaProntos* fila_alta, FilaProntos* fila_baixa);

#endif // IO_H
