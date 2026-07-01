#include "constants.h"
#include "prontos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PCB* inicializaProcesso(int pid){

    PCB* novo_processo = calloc(1, sizeof(*novo_processo));
    if (novo_processo == NULL) {
        return NULL;
    }

    novo_processo->PID = pid;
    novo_processo->status = NOVO;
    novo_processo->prioridade = ALTA;
    novo_processo->tempo_na_cpu = 0;
    novo_processo->tempo_servico = 0;
    novo_processo->quant_IO = 0;
    novo_processo->instante_chegada = 0;
    novo_processo->instante_saida = 0;

    return novo_processo;
}

void inicializaFila(FilaProcessos* F) {
    if (F == NULL) {
        return;
    }

    F->fim = NULL;
    F->inicio = NULL;
    F->tamanho = 0;
}

void novoProcesso(FilaProcessos *fila, PCB* pcb) {
    if (fila == NULL || pcb == NULL) {
        return;
    }

    Processo *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return;
    }

    novo->pcb = *pcb;
    novo->proximo = NULL;

    if (fila->fim) {
        fila->fim->proximo = novo;
    } else {
        fila->inicio = novo;
    }
    fila->fim = novo;
    fila->tamanho++;
    free(pcb);
}

// lê a tabela.txt e carrega os valores numa fila de processos
// Primeira linha: ignorada (cabeçalho)
// Segunda linha: quantidade de processos
// Linhas seguintes: instante_chegada tempo_servico [quantidade_ios tipo1 momento1 tipo2 momento2 ...]
void lerLista(FilaProcessos* fila) {

    FILE *arquivo = fopen(PATH, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo %s\n", PATH);
        return;
    }

    char linha[128];
    int num_processos = 0;
    int indice = 100;

    // Pula primeira linha (cabeçalho)
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return;
    }

    // Lê a quantidade de processos
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return;
    }

    if (sscanf(linha, "%d", &num_processos) != 1) {
        fclose(arquivo);
        return;
    }

    for (int i = 0; i < num_processos; i++) {
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            break;
        }

        int valores[64] = {0};
        int qtd_valores = 0;
        char *token = strtok(linha, " \t\n");

        while (token != NULL && qtd_valores < 64) {
            valores[qtd_valores++] = atoi(token);
            token = strtok(NULL, " \t\n");
        }

        if (qtd_valores < 2) {
            continue;
        }

        PCB *pcb = inicializaProcesso(indice);
        if (pcb == NULL) {
            continue;
        }

        pcb->instante_chegada = valores[0];
        pcb->tempo_servico = valores[1];

        int qtd_ios = 0;
        if (qtd_valores >= 3) {
            qtd_ios = valores[2];
        }

        if (qtd_ios > MAX_IO_POR_PROCESSO) {
            qtd_ios = MAX_IO_POR_PROCESSO;
        }

        pcb->quant_IO = qtd_ios;

        for (int j = 0; j < qtd_ios; j++) {
            int indice_valor = 3 + (2 * j);
            if (indice_valor + 1 >= qtd_valores) {
                break;
            }

            int tipo_valor = valores[indice_valor];
            int momento = valores[indice_valor + 1];

            if (tipo_valor == IO_DISCO || tipo_valor == 1) {
                pcb->lista_ios[j].tipo = DISCO;
            } else if (tipo_valor == IO_FITA || tipo_valor == 2) {
                pcb->lista_ios[j].tipo = FITA;
            } else if (tipo_valor == IO_IMPRESSORA || tipo_valor == 3) {
                pcb->lista_ios[j].tipo = IMPRESSORA;
            } else {
                pcb->lista_ios[j].tipo = DISCO;
            }

            pcb->lista_ios[j].momento_interrupcao = momento;
        }

        novoProcesso(fila, pcb);
        indice++;
    }
    fclose(arquivo);
}

PCB* popProcesso(FilaProcessos* F) {
    if (F == NULL || F->inicio == NULL) {
        return NULL;
    }

    Processo* primeiro = F->inicio;
    PCB* pcb = calloc(1, sizeof(*pcb));
    if (pcb == NULL) {
        return NULL;
    }

    *pcb = primeiro->pcb;

    F->inicio = primeiro->proximo;
    if (F->inicio == NULL) {
        F->fim = NULL;
    }
    F->tamanho--;
    free(primeiro);

    return pcb;
}

