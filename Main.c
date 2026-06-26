#include <stdio.h>
#include <stdlib.h>

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
}Processo;

Processo iniciaProcesso(int pid, int ppid){

}

void main(){
  printf("Hello world");
}

