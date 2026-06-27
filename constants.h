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
    DISCO, 
    FITA, 
    IMPRESSORA
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
  OperacaoIO lista_ios[MAX_IO_POR_PROCESSO]; // lembrar de definir limites de ios pra nao dar ruim - ver com luz
} PCB;

typedef struct{ // pra saber qm ta na cpu e qnt tempo ta executando
  PCB* processo_executando;
  int tempo_executado;
} Execucao;

