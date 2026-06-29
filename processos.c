#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include "prontos.c"

PCB* inicializaProcesso(int pcb){

    PCB* novo_processo = (PCB*) malloc(sizeof(PCB));

    novo_processo->PID=pcb;
    novo_processo->status=NOVO;
    novo_processo->prioridade=ALTA;
    novo_processo->tempo_na_cpu=0;

    novo_processo->tempo_servico = (rand()%20+5); // 0 a 19 em sorteio com min de 5: 5-24 u.t

    int quant_IO = (rand()%4);

    for (int i = 0; i<quant_IO;i++){
        novo_processo->lista_ios[i].tipo = rand()%3; //0:dicso/1:fita/2:impressora
        novo_processo->lista_ios[i].momento_interrupcao = rand()%novo_processo->tempo_servico;
    }   
    return novo_processo;
}

void inicializaFila(FilaProcessos* F) {
    F->fim = NULL;
    F->inicio = NULL;
    F->tamanho = 0;
}

void novoProcesso(FilaProcessos *fila, PCB* pcb) {
    Processo *novo = malloc(sizeof(Processo));

    novo->pcb = *pcb;
    novo->proximo = NULL;
    free(pcb);

    if (fila->fim) {
        fila->fim->proximo = novo;
    } else {
        fila->inicio = novo;
    }
    fila->fim = novo;
    fila->tamanho++;
}

// lê a tabela.txt e carrega os valores numa fila de processos na ordem dos indices
void lerLista(FilaProcessos* fila) {

    FILE *arquivo = fopen (PATH, "r");

    char linha[MAX_PROCESSOS];
    int indice = 1; // começa a ler da segunda linha, pq na primeira tem instruções

    while (fgets(linha, sizeof(linha), arquivo)) {
        PCB pcb = *inicializaProcesso(indice);

        sscanf(linha, "%d %d", &pcb.instante_chegada, &pcb.tempo_servico);
            novoProcesso(fila, &pcb);
    }
    fclose(arquivo);
}

Processo* popProcesso(FilaProcessos F) {
    Processo* primeiro = F.inicio;
    F.inicio = F.inicio->proximo;
    return(primeiro);
}