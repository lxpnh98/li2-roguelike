#include <stdio.h>

#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

#define MAX_BUFFER        10240

#define TAM               9

#define ESCALA            40

#define NUM_TIPO_INIMIGOS       3

/**
\brief Estrutura que armazena uma posição
*/
typedef struct posicao {
	int x;
	int y;
} POSICAO;

typedef struct inimigo {
    enum {GUERREIRO, CORREDOR, CAVALEIRO} tipo;
    POSICAO pos;
} INIMIGO;

typedef struct jogador {
	POSICAO pos;
	int score;
	int vidas;
	enum {NORMAL     = 'n', 
              ATAQUE     = 'a',
              MUDAR_MODO = 'm'} modo;
} JOGADOR;
/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	/** \brief A posição do jogador */
	JOGADOR jog;
	/** \brief O nº de inimigos */
	char num_inimigos;
	/** \brief O nº de obstáculos */
	char num_obstaculos;
	/** \brief Array com a posição dos inimigos */
	INIMIGO inimigo[MAX_INIMIGOS];
	/** \brief Array com a posição dos obstáculos */
	POSICAO obstaculo[MAX_OBSTACULOS];
        /** \brief Saída do nível */
        POSICAO saida;
} ESTADO;

/**
\brief Função que converte um estado numa string
@param e O estado
@returns A string correspondente ao estado e
*/
char *estado2str(ESTADO e);

/**
\brief Função que converte uma string num estado 
@param argumentos Uma string contendo os argumentos passados à CGI
@returns O estado correspondente à string dos argumentos
*/
ESTADO str2estado(char *argumentos);

ESTADO ler_estado(FILE *file, char query[]);

void guardar_estado(FILE *fp, ESTADO e);

ESTADO atualizar_estado(ESTADO e, char query[]);

#endif
