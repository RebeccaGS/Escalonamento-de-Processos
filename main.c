#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "inicializar_processos.c"


PCB iniciaProcesso(int pid, int ppid);
void enviaPraFila(PCB* processo, PCB** fila_alta);
int testeInicialMalloc(void* array);
PCB* escolheProximo(PCB** fila_alta, PCB** Fila_baixa);


void main(){

  srand((unsigned)time(NULL)); // evita q rand() sorteie msm num sempre

  int numero_processos;
  printf("Digite quantos processos: \n");
  scanf("%d", &numero_processos);
  if (numero_processos > MAX_PROCESSOS){
    printf("Número pedido excedeu limite. Redefinido número de processos para valor máximo: = %d",MAX_PROCESSOS);
    numero_processos = MAX_PROCESSOS;
  }

  PCB** fila_alta = malloc(numero_processos * sizeof(PCB*));
  testeInicialMalloc(fila_alta);
  PCB** fila_baixa = malloc(numero_processos * sizeof(PCB*));
  testeInicialMalloc(fila_baixa);

  for (int i = 100; i < numero_processos+100; i++){ // fiz o pid começar em 100 pra evitar pids reservados p/ o kerbel
    PCB processo_novo = iniciaProcesso(i,1); // defini todos como filhos do processo raiz (ppid = 1)
    enviaPraFila(&processo_novo,fila_alta);
  }
  

  //Joga todos os processos pra fila de alta de início
  //Depois a gente pensa na melhor forma de fazer essa leitura de processos
  for(int PPID = 1; PPID <= numero_processos; PPID++){ 
    PCB processo = iniciaProcesso(PPID, 1);
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
    

  }
}


int testeInicialMalloc(void* array){
  if(array == NULL) {
        printf("Erro de memoria!\n");
        return 1;
    }
}