#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "processos.h"
#include "prontos.h"
#include "IO.h"
#include "saida.h"


int main(void){

    // Inicialização das variáveis
    srand((unsigned)time(NULL)); // evita q rand() sorteie msm num sempre

    int tempo_global = 0;
    FilaProntos *fila_alta = malloc(sizeof(FilaProntos));
    FilaProntos *fila_baixa = malloc(sizeof(FilaProntos));

    FilaIO fila_fita, fila_impressora, fila_disco;
    FilaProcessos fila_entrada, fila_saida;
    inicializaFila(&fila_entrada);
    inicializaFila(&fila_saida);
    lerLista(&fila_entrada);

    int total_processos = fila_entrada.tamanho;
    inicializaFilaPronto(fila_alta, ALTA);
    inicializaFilaPronto(fila_baixa, BAIXA);
    inicializaFilaIO(&fila_disco);
    inicializaFilaIO(&fila_fita);
    inicializaFilaIO(&fila_impressora);

    FilaIO filas[3] = {fila_disco, fila_fita, fila_impressora};
    PCB* emCPU = NULL;
    int fatia = 0;

    int execucao[TEMPO_MAX]; // trem q faz a tabela, execucao[t] = PID que estava rodando no instante t (-1 se CPU ociosa)
    int pids[MAX_PROCESSOS]; // pra saber quais PIDs tem q printar na tabela
    int pids_unicos = 0;
    // Ciclo principal
    while (1) {

        // Chegada de novos processos no instante atual
        while (fila_entrada.inicio != NULL &&
                fila_entrada.inicio->pcb.instante_chegada == tempo_global) {
            PCB *proc = popProcesso(&fila_entrada);
            if (proc != NULL) {
                int pid = proc->PID;      // captura o PID de quem chegou
                novoPronto(fila_alta, proc); // proc é liberado aqui dentro
                pids[pids_unicos++] = pid;   // usa a cópia segura
            }
        }

        // Verifica se a simulação terminou
        if (fila_saida.tamanho == total_processos &&
            emCPU == NULL &&
            fila_alta->tamanho == 0 &&
            fila_baixa->tamanho == 0 &&
            fila_entrada.inicio == NULL) {
            break;
        }

        // Escolhe o próximo processo
        if (emCPU == NULL) {
            if (fila_alta->tamanho > 0 || fila_baixa->tamanho > 0) {
                emCPU = escolheProximo(fila_alta, fila_baixa);
                fatia = 0;
            } else {
                // Não há processo pronto: só atualiza IO e avança o tempo
                gerenciaFilaIO(filas, fila_alta, fila_baixa);
                tempo_global++;
                continue;
            }
        }

        // carrega quem tá na cpu quando pra montar a tabela de execução
        if (emCPU != NULL) {
            execucao[tempo_global] = emCPU->PID;
        } else {
            execucao[tempo_global] = -1; // CPU ociosa
        }

        // Segurança para evitar usar CPU quando não há processo
        if (emCPU == NULL) {
            gerenciaFilaIO(filas, fila_alta, fila_baixa);
            tempo_global++;
            continue;
        }

        //  Preempção por quantum
        if (fatia >= QUANTUM) {
            emCPU->status = PRONTO;
            novoPronto(fila_baixa, emCPU);
            emCPU = NULL;
            fatia = 0;
            gerenciaFilaIO(filas, fila_alta, fila_baixa);
            tempo_global++;
            continue;
        }

        // Execução de uma unidade de tempo do processo atual
        emCPU->status = EXEC;
        fatia++;
        emCPU->tempo_na_cpu++;

        // Verifica término do processo + debug
        if (emCPU->tempo_na_cpu >= emCPU->tempo_servico) {
            emCPU->status = FINALIZADO;
            emCPU->instante_saida = tempo_global + 1;
            novoProcesso(&fila_saida, emCPU);
            emCPU = NULL;
            fatia = 0;
            
        } else { // Se o processo não terminou, verifica se precisa de IO
            for (int i = 0; i < emCPU->quant_IO; i++) {
                if (emCPU->lista_ios[i].momento_interrupcao == emCPU->tempo_na_cpu) {
                    TipoIO tipo = emCPU->lista_ios[i].tipo;
                    for (int j = i; j + 1 < emCPU->quant_IO; j++) {
                        emCPU->lista_ios[j] = emCPU->lista_ios[j + 1];
                    }
                    emCPU->quant_IO--;
                    bloqueioIO(filas, emCPU, tipo);
                    emCPU = NULL;
                    fatia = 0;
                    break;
                }
            }
        }

        if (emCPU == NULL) {
            gerenciaFilaIO(filas, fila_alta, fila_baixa);
            tempo_global++;
            continue;
        }

        // Atualiza filas de IO e avança o tempo global
        gerenciaFilaIO(filas, fila_alta, fila_baixa);
        tempo_global++;
    }

    imprimirMatriz(execucao, tempo_global, pids, total_processos);
}
