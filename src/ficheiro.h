/**
@file ficheiro.h
Definição de funções de conversão entre string e ESTADO, de leitura e escrita do estado e das pontuações.
*/

#include "estado.h"

/**
\brief Função que converte uma string num estado 
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

ESTADO ler_estado(FILE *file, char query[]);

void guardar_estado(FILE *fp, ESTADO e);

void guardar_pontuacao(char top_scores[], JOGADOR jog);

