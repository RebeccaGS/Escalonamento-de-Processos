#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "IO.c"
#include "constants.h"
#include "processos.c"
#include "prontos.c"


void main(void){

    // Inicialização das variáveis
    srand((unsigned)time(NULL)); // evita q rand() sorteie msm num sempre

    int numero_processos = MAX_PROCESSOS;
    int processos_criados = 0;
    int tempo_global = 0;
    FilaProntos *fila_alta = malloc(sizeof(FilaProntos)), 
                *fila_baixa = malloc(sizeof(FilaProntos));

    FilaIO fila_fita, fila_impressora, fila_disco;
    FilaProcessos fila_processos;
    lerLista(&fila_processos);

    inicializaFilaPronto(&fila_alta, ALTA);
    inicializaFilaPronto(&fila_baixa, BAIXA);
    inicializaFilaIO(&fila_disco);
    inicializaFilaIO(&fila_fita);
    inicializaFilaIO(&fila_impressora);

    FilaIO filas[3] = {fila_disco, fila_fita, fila_impressora};
    PCB* emCPU = malloc(sizeof(PCB));
    int fatia = 0;

    // Ciclo principal
    while(1){
        
    tempo_global++;

    //todo: aqui é pra ele conferir se tem processo na fila_processos pra entrar na fila_prontos

    // Caso sem processo, fetch novo processo.
    if(emCPU == NULL){
      emCPU = escolheProximo(&fila_alta, &fila_baixa);
    }

    // Testa IO
    for (int i = 0; i < emCPU->quant_IO; i++) {
        if (emCPU->lista_ios[i].momento_interrupcao == emCPU->tempo_na_cpu) {
            bloqueioIO(filas, emCPU, emCPU->lista_ios[i].tipo); // bloqueio limpa o emCPU
            continue;
        }
    }

    // Testa Quantum
    if(fatia > QUANTUM) {
        novoPronto(fila_baixa, emCPU);
        fatia = 0;
        continue;
    }; //novoPronto tb faz emCPU virar null
    
    // Se não tiver IO, e ainda estiver no Quantum, faz update dos valores.
    fatia++;
    emCPU->tempo_na_cpu++;

    // Atualiza Filas
    gerenciaFilaIO(filas, fila_alta, fila_baixa);

  }
}