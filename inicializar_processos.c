#include "constants.h"

PCB iniciaProcesso(int pid, int ppid){
    PCB novo_processo;

    novo_processo.PID=pid;
    novo_processo.PPID=ppid;
    novo_processo.status=NOVO;
    novo_processo.prioridade=ALTA;

    novo_processo.tempo_servico = (rand()%20+5); // 0 a 19 em sorteio com min de 5: 5-24 u.t
    novo_processo.quant_IO = (rand()%4);
    novo_processo.lista_ios;

    for (int i = 0; i<novo_processo.quant_IO;i++){
        novo_processo.lista_ios[i].tipo = rand()%3; //0:dicso/1:fita/2:impressora
        novo_processo.lista_ios[i].momento_interrupcao = rand()%novo_processo.tempo_servico;
        switch(novo_processo.lista_ios[i].tipo) {
            case DISCO:
                novo_processo.lista_ios[i].duracao = 4; //4 u.t.
                break;
            case FITA:
                novo_processo.lista_ios[i].duracao = 6;
                break;
            case IMPRESSORA:
                novo_processo.lista_ios[i].duracao = 8;
                break;
        }
    }   
    return novo_processo;
}
