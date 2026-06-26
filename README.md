# Escalonamento-de-Processos


## rascunho do luz sobre a modelagem do programa

Tabelas com os processos, armazenada em um arquivo.
O processo na tabela terá:
- tempo de CPU, int
- lista de IO, cada io tendo um tempo de quando ele ocorre, e um tipo de IO. array de vetores 2d

Lógica do programa:

Loop principal enquanto houver processos a entrarem: cada iteração equivale a uma unidade de tempo.

Atualização da fila de prontos:
    Verifica processos novos naquele momento
    Atualiza a fila de IO

Consumir os processos:
    Se acabar a fatia de tempo, consumir o próximo
    Se o processo for interrompido, consumir o próximo
    Se o processo terminar, consumir o próximo

Tabela de saída: pontos de dados criados a cada unidade de tempo (processos na fila, processos em IO, o estado da maquina)
Esses pontos de dados servirão pra gerar a tabela bonitinha no final com as informações mais pertinentes igual ela mostra em aula

Sugestão de implementação a nível de código:

constants.c -> constantes
input.c -> trata da leitura da entrada e criação das structs dos processos
process.c -> enum/struct dos processos
IO.c -> trata dos bloqueios por IO
main.c -> main
output.c? -> produz a tabelinha de saída

Dá pra simplificar bastante esse codigo, se for fazer só o básico dá pra colocar tudo na main num while bem simples por exemplo. Ao mesmo tempo dá pra separar em um monte de função e arquivos diferentes, acho que o ideal seria algo entre os dois.
