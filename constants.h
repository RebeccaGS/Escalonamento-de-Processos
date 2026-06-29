#define QUANTUM 4
#define MAX_IO_POR_PROCESSO 5
#define MAX_PROCESSOS 20


typedef enum{
  ALTA, BAIXA
} Prioridade;

typedef enum{
  NOVO, PRONTO, EXEC, BLOQUEADO, FINALIZADO
} Status;

typedef enum {
    DISCO = 5, 
    FITA = 10, 
    IMPRESSORA = 20
} TipoIO;

typedef struct {
    TipoIO tipo;
    int momento_interrupcao; 
    int duracao;             
} OperacaoIO;

typedef struct { 
  int PID;
  int PPID;
  Prioridade prioridade; 
  Status status;
  int tempo_servico;
  int quant_IO;
  int instante_chegada;
  int tempo_na_cpu; // pra poder bater com os tempos de IO e tbm pra fazer a tabela dps
  OperacaoIO lista_ios[MAX_IO_POR_PROCESSO]; // lembrar de definir limites de ios pra nao dar ruim - ver com luz
} PCB;

typedef struct{ // pra saber qm ta na cpu e qnt tempo ta executando
  PCB* processo_executando;
  int tempo_executado;
} Execucao;

// parte das filas de IO e Prontos

typedef struct {
  PCB pcb;
  ProcessoIO *proximo;
  TipoIO tipo;
  int espera;
} ProcessoIO;

typedef struct {
  int tamanho;
  ProcessoIO* inicio;
  ProcessoIO* fim;
} FilaIO;

typedef struct {
  PCB pcb;
  ProcessoPronto *proximo;
} ProcessoPronto;

typedef struct {
  int tamanho;
  ProcessoIO* inicio;
  ProcessoIO* fim;
  Prioridade prioridade;
} FilaProntos;