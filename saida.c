#include <stdio.h>
#include "constants.h"

void imprimirMatriz(int execucao[], int tempo_total, int pids[],int qtd_processos,FilaProcessos *fila_saida) {
    // tabela 1 q mostra fluxo da cpu a cada u.t.
    printf("CPU a longo do tempo:\n\n");
    printf("%-5s", "PID");
    for (int t = 0; t < tempo_total; t++) {
        printf("%-3d", t);
    }
    printf("\n");

    
    for (int i = 0; i < qtd_processos; i++) {
        int pid_atual = pids[i];
        printf("%-5d", pid_atual);

        for (int t = 0; t < tempo_total; t++) {
            char marcador = (execucao[t] == pid_atual) ? 'X' : '.';
            printf("%-3c", marcador);
        }

        printf("\n");
    }

    printf("%-5s", "Ocio");

    for (int t = 0; t < tempo_total; t++) {
        char marcador = (execucao[t] == -1) ? 'X' : '.';
        printf("%-3c", marcador);
    }
    printf("\n");

    printf("\nResumo da execucao:\n\n");
    Processo *p = fila_saida->inicio;

    int tempo_ocioso = 0;

    for (int t = 0; t < tempo_total; t++) {
        if (execucao[t] == -1)  tempo_ocioso++;  }

    while (p != NULL) {
        int turnaround = p->pcb.instante_saida - p->pcb.instante_chegada;
        printf("PID %2d | Entrada: %2d | Saida: %2d | Turnaround: %2d\n",p->pcb.PID,p->pcb.instante_chegada,p->pcb.instante_saida,turnaround);
        p = p->proximo;
    }

    printf("\nCPU Ociosa | Tempo total ocioso: %d\n", tempo_ocioso);
}



    
