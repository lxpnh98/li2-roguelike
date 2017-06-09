/**
@file estado.h
Definição do estado, de contantes relevantes e das funções que o inicializam e atualizam.
*/

#include <stdio.h>

#ifndef ___ESTADO_H___
#define ___ESTADO_H___

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

/** \brief O nº máximo de caracteres em buffer */
#define MAX_BUFFER              10240

/** \brief Tamanho do mapa */
#define TAM                     9

/** \brief Escala das imagens das casas */
#define ESCALA                  40

/** \brief O nº de tipos de inimigos */
#define NUM_TIPO_INIMIGOS       3

/** \brief O nº inicial de vidas do jogador */
#define INIT_VIDAS              5

/** \brief A constante que serve para indicar se a funcão rand_pos() testa pela saída */
#define TESTAR_SAIDA            1

/** \brief Estrutura que armazena uma posição */
typedef struct posicao {
        /** \brief A coordenada X da posição */
	int x;
        /** \brief A coordenada Y da posição */
	int y;
} POSICAO;

/** \brief Estrutura que representa um inimigo */
typedef struct inimigo {
    /** \brief O tipo do inimigo */
    enum {GUERREIRO, CORREDOR, CAVALEIRO} tipo;
    /** \brief A posição do inimigo */
    POSICAO pos;
} INIMIGO;

/** \brief Estrutura que representa o jogador */
typedef struct jogador {
    /** \brief A posição do jogador */
    POSICAO pos;
    /** \brief A pontuação do jogador */
    int score;
    /** \brief O nº de vidas do jogador */
    int vidas;
    /** \brief O modo de jogo do estado */
    enum {NORMAL     = 'n', 
          ATAQUE     = 'a',
          MUDAR_MODO = 'm'} modo; // Mudar para estado.
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

ESTADO atualizar_estado(ESTADO e, char query[]);

ESTADO inicializar(int vidas, int score);

#endif
