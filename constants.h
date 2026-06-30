#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TEMPO_MAX 100 
#define QUANTUM 4
#define MAX_IO_POR_PROCESSO 5
#define MAX_PROCESSOS 30
#define PATH "tabela.txt"
#define IO_DISCO 5
#define IO_FITA 10
#define IO_IMPRESSORA 20

typedef enum{
  ALTA, BAIXA
} Prioridade;

typedef enum{
  NOVO, PRONTO, EXEC, BLOQUEADO, FINALIZADO
} Status;

typedef enum {
    DISCO = 3, 
    FITA = 6, 
    IMPRESSORA = 9
} TipoIO;

typedef struct {
    TipoIO tipo;
    int momento_interrupcao;         
} OperacaoIO;

typedef struct { 
  int PID;
  Prioridade prioridade; 
  Status status;
  int tempo_servico;
  int quant_IO;
  int instante_chegada;
  int instante_saida;
  int tempo_na_cpu; // pra poder bater com os tempos de IO e tbm pra fazer a tabela dps
  OperacaoIO lista_ios[MAX_IO_POR_PROCESSO]; // lembrar de definir limites de ios pra nao dar ruim - ver com luz
} PCB;

// parte das filas de IO e Prontos

typedef struct ProcessoIO{
  PCB pcb;
  struct ProcessoIO *proximo;
  TipoIO tipo;
  int espera;
} ProcessoIO;

typedef struct {
  int tamanho;
  ProcessoIO* inicio;
  ProcessoIO* fim;
} FilaIO;

typedef struct ProcessoPronto{
  PCB pcb;
  struct ProcessoPronto *proximo;
} ProcessoPronto;

typedef struct {
  int tamanho;
  ProcessoPronto* inicio;
  ProcessoPronto* fim;
  Prioridade prioridade;
} FilaProntos;

typedef struct Processo {
  PCB pcb;
  struct Processo* proximo;
} Processo;

typedef struct {
  int tamanho;
  Processo* inicio;
  Processo* fim;
} FilaProcessos;

#endif // CONSTANTS_H