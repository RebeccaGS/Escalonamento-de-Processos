#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

typedef enum{
  ALTA, BAIXA
} Prioridade;

typedef enum{
  PRONTO, EXEC, BLOQUEADO
}Status;

typedef struct
{
  int PID;
  int PPID;
  Prioridade prioridade; 
  Status status;
  IO tipo_io;
  int tempo_servico;
}Processo;

typedef struct{
  Processo* processo_executando;
  int tempo_executado;
}Execucao;

Processo iniciaProcesso(int pid, int ppid);

void enviaPraFila(Processo* processo, Processo** fila_alta);

int testeInicialMalloc(void* array);

Processo* escolheProximo(Processo** fila_alta, Processo** Fila_baixa);



void main(){
    struct timespec tempo;
    tempo.tv_sec = 0;
    tempo.tv_nsec = (8*1000000);  // DEPOIS TROCA AQUI PELO SLICE

    int numero_processos;

    printf("Digite quantos processos: \n");
    scanf("%d", &numero_processos);

    Processo** fila_alta = malloc(numero_processos * sizeof(Processo));
    testeInicialMalloc(fila_alta);
    Processo** fila_baixa = malloc(numero_processos * sizeof(Processo));
    testeInicialMalloc(fila_baixa);

    //Joga todos os processos pra fila de alta de início
    //Depois a gente pensa na melhor forma de fazer essa leitura de processos
    for(int PPID = 1; PPID <= numero_processos; PPID++){ 
      Processo processo = iniciaProcesso(PPID, 1);
      enviaPraFila(&processo, fila_alta);
    }

    Execucao execucao;
  while(1){
    // VERIFICAR SE A ORDEM DE TUDO TÁ CERTINHA

    if(execucao.processo_executando == NULL){
      //Inicia um novo processo na execução e seta o tempo do quantum
      execucao.processo_executando = escolheProximo(&fila_alta, &fila_baixa);
      execucao.tempo_executado = QUANTUM;
    }
    
    //VAI TRABALHAR CPU VAGABUNDA
    if(execucao.processo_executando != NULL){
      execucao.processo_executando->tempo_servico--;
      execucao.tempo_executado--; //LEMBRAR DE INICIALIZAR PROCESSO COM VALOR DE QUANTUM

      //Se acabou o tempo de serviço do processo -> dropa
      if(execucao.processo_executando->tempo_servico == 0){
        execucao.processo_executando = NULL;
      }

      //Se acabou o quantum -> vai pra fila de baixa
      if(execucao.tempo_executado == 0){
        enviaPraFila(execucao.processo_executando, fila_baixa);
        execucao.processo_executando = NULL;
      }
      //AQUI LUZ BOTA O I/O AQUI
    }

    
    // DIMINUIR O TEMPO DE QUEM TÁ NO BLOQUEADO EM I/O
    
    nanosleep(&tempo, NULL);
  }
}


int testeInicialMalloc(void* array){
  if(array == NULL) {
        printf("Erro de memoria!\n");
        return 1;
    }
}