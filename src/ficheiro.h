/**
@file ficheiro.h
Definição de funções de conversão entre string e ESTADO, de leitura e escrita do estado e das pontuações.
*/

#include "estado.h"

ESTADO str2estado(char *argumentos);

char *estado2str(ESTADO e);

ESTADO ler_estado(FILE *file, char query[]);

void guardar_estado(FILE *fp, ESTADO e);

void guardar_pontuacao(char top_scores[], JOGADOR jog);

