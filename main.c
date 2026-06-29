#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "IO.c"
#include "constants.h"
#include "inicializar_processos.c"
#include "prontos.c"
#include "IO.c"


PCB iniciaProcesso(int pid, int ppid);
int testeInicialMalloc(void* array);


void main(){
  int tempo_global = 0;
  srand((unsigned)time(NULL)); // evita q rand() sorteie msm num sempre

  int numero_processos;
  printf("Digite quantos processos: \n");
  scanf("%d", &numero_processos);
  if (numero_processos > MAX_PROCESSOS){
    printf("Número pedido excedeu limite. Redefinido número de processos para valor máximo: = %d",MAX_PROCESSOS);
    numero_processos = MAX_PROCESSOS;
  }

  int processos_finalizados = 0; // Pra poder verificar quando o programa para de rodar

  FilaProntos fila_alta, fila_baixa;
  FilaIO fila_fita, fila_impressora, fila_disco;

  inicializaFilaPronto(&fila_alta, ALTA);
  inicializaFilaPronto(&fila_baixa, BAIXA);
  inicializaFilaIO(&fila_disco);
  inicializaFilaIO(&fila_fita);
  inicializaFilaIO(&fila_impressora);

  for (int i = 100; i < numero_processos+100; i++){ // fiz o pid começar em 100 pra evitar pids reservados p/ o kerbel
    PCB processo_novo = iniciaProcesso(i,1); // defini todos como filhos do processo raiz (ppid = 1)
    processo_novo.instante_chegada = tempo_global;
    enviaPraFila(&fila_alta, &processo_novo);
  }

  Execucao execucao;


  while(1){
    // VERIFICAR SE A ORDEM DE TUDO TÁ CERTINHA

    if(execucao.processo_executando == NULL){
      //Inicia um novo processo na execução e seta o tempo do quantum
      execucao.processo_executando = escolheProximo(&fila_alta, &fila_baixa);
      if(execucao.processo_executando != NULL){
        execucao.tempo_executado = QUANTUM;

        // TODO: VER SE VAI TER PRINT
      }
    }
    
    //VAI TRABALHAR CPU VAGABUNDA
    if(execucao.processo_executando != NULL){
      PCB* p = execucao.processo_executando;
      int pediu_io = 0; // SERVE PRA NÃO DAR PROBLEMA NAS COISAS DPS QUE O PONTEIRO VIRAR NULL EM CASO DE BLOQUEIO

      for(int i=0; i<p->quant_IO;i++){ //percorre a lista de tempo pro io
        if(p->tempo_na_cpu == p->lista_ios[i].momento_interrupcao){
          
          TipoIO tipo = p->lista_ios[i].tipo;
          if (tipo == DISCO) bloqueioIO(&fila_disco, p, tipo);
          else if (tipo == FITA) bloqueioIO(&fila_fita, p, tipo);
          else bloqueioIO(&fila_impressora, p, tipo);

          // TODO: ver se vai ter algum aviso na tela 
          execucao.processo_executando = NULL;
          pediu_io = 1;
          break;
        }
      } 

      if(pediu_io == 0){
        execucao.processo_executando->tempo_na_cpu++;
        execucao.processo_executando->tempo_servico--;
        execucao.tempo_executado--; //LEMBRAR DE INICIALIZAR PROCESSO COM VALOR DE QUANTUM

        //Se acabou o tempo de serviço do processo -> dropa
        if(execucao.processo_executando->tempo_servico == 0){
          execucao.processo_executando = NULL;
          processos_finalizados++;

          if(processos_finalizados == numero_processos){
            //finaliza();
            return 0;
          }

        }

        //Se acabou o quantum -> vai pra fila de baixa
        if(execucao.tempo_executado == 0){
          enviaPraFila(execucao.processo_executando, fila_baixa);
          execucao.processo_executando = NULL;
        }
        
      }
    }
    
    gerenciaFilaIO(&fila_disco, &fila_alta, &fila_baixa);
    gerenciaFilaIO(&fila_fita, &fila_alta, &fila_baixa);
    gerenciaFilaIO(&fila_impressora, &fila_alta, &fila_baixa);

    tempo_global++;
  }
}


int testeInicialMalloc(void* array){
  if(array == NULL) {
        printf("Erro de memoria!\n");
        return 1;
    }
}


PCB* escolheProximo(FilaProntos* A, FilaProntos* B) {
  if (A->tamanho != 0) {
    return pop(A);
  } else {
    return pop(B);
  }
}