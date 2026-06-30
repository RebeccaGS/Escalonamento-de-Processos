#include <stdio.h>

void imprimirMatriz(int execucao[], int tempo_total, int pids[], int qtd_processos) {
    // Cabeçalho: eixo X (tempo)
    printf("%-5s", "PID");
    for (int t = 0; t < tempo_total; t++) {
        printf("%-3d", t);
    }
    printf("\n");

    // Uma linha por PID (eixo Y)
    for (int i = 0; i < qtd_processos; i++) {
        int pid_atual = pids[i];
        printf("%-5d", pid_atual);

        for (int t = 0; t < tempo_total; t++) {
            char marcador = (execucao[t] == pid_atual) ? 'X' : '.';
            printf("%-3c", marcador);
        }
        printf("\n");
    }
}