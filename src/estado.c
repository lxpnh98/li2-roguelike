/**
@file estado.c
Funções de inicialização e atualização do estado.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "estado.h"
#include "calc.h"
#include "ficheiro.h"

/**
\brief Função que adiciona um inimigo ao mapa.
@param e Estado do jogo anterior.
@returns ESTADO Novo estado do jogo.
*/
ESTADO inicializar_inimigo(ESTADO e) {
    POSICAO p;
    rand_pos(e, &p, TESTAR_SAIDA);
    e.inimigo[(int)e.num_inimigos].tipo = rand() % NUM_TIPO_INIMIGOS;
    e.inimigo[(int)e.num_inimigos].pos = p;
    e.num_inimigos++;
    return e;
}

/**
\brief Função que inicializa a posição dos inimigos.
@param e Estado do jogo anterior.
@param num Número de inimigos a serem colocados no mapa.
@returns ESTADO Novo estado do jogo.
*/
ESTADO inicializar_inimigos(ESTADO e, int num) {
    int i;
    for (i = 0; i < num; i++)
        e = inicializar_inimigo(e);
    return e;
}

/**
\brief Função que adiciona um obstáculo ao mapa.
@param e Estado do jogo anterior.
@returns ESTADO Novo estado do jogo.
*/
ESTADO inicializar_obstaculo(ESTADO e) {
    POSICAO p;
    rand_pos(e, &p, TESTAR_SAIDA);
    e.obstaculo[(int)e.num_obstaculos] = p;
    e.num_obstaculos++;
    return e;
}

/**
\brief Função que inicializa a posição dos obstáculos.
@param e Estado do jogo anterior.
@param num Número de obstáculos a serem colocados no mapa.
@returns ESTADO Novo estado do jogo.
*/
ESTADO inicializar_obstaculos(ESTADO e, int num) {
    int i;
    for (i = 0; i < num; i++)
        e = inicializar_obstaculo(e);
    return e;
}

/**
\brief Função que inicializa um novo nível do jogo.
@param vidas Número de vidas do jogador ao entrar no nível.
@param score Pontuação do jogador ao entrar no nível.
@returns ESTADO Novo estado do jogo.
*/
ESTADO inicializar(int vidas, int score) {
    POSICAO p;
    ESTADO e = {{{0,0},0,0,0},0,0,{{0}},{{0}},{0,0}};
    rand_pos(e, &p, !TESTAR_SAIDA);
    e.jog.pos = p;
    e.jog.vidas = vidas;
    e.jog.score = score;
    e.jog.modo = NORMAL;
    rand_pos(e, &p, !TESTAR_SAIDA);
    e.saida = p;
    e = inicializar_inimigos(e, 10);
    e = inicializar_obstaculos(e, 10);
    return e;
}

/**
\brief Função que preenche a matriz de movimento.
@param m Matriz a preencher.
@param e Estado do jogo.
@param x Coordenada X da posição a preencher.
@param y Coordenada Y da posição a preencher.
@param dist Distância do jogador à posição atual.
*/
void preencher(int m[TAM][TAM], ESTADO e, int x, int y, int dist)
{
    POSICAO p;
    p.x = x;
    p.y = y;
    if (m[y][x] > dist && posicao_valida(p) && 
        !tem_obstaculo(e, p) && !tem_inimigo(e, p)) {
        m[y][x] = dist;
        preencher(m, e, x + 1, y    , dist + 1);
        preencher(m, e, x - 1, y    , dist + 1);
        preencher(m, e, x    , y - 1, dist + 1);
        preencher(m, e, x    , y + 1, dist + 1);
        preencher(m, e, x + 1, y - 1, dist + 1);
        preencher(m, e, x - 1, y + 1, dist + 1);
    }
}

/**
\brief Função que constrói a matriz de movimento.
@param e Estado do jogo.
@param m Matriz a preencher.
*/
void matriz_guerreiro(ESTADO e, int m[TAM][TAM])
{
    int i, j;
    for (i = 0; i < TAM; i++)
        for (j = 0; j < TAM; j++)
            m[i][j] = 999;
    preencher(m, e, e.jog.pos.x, e.jog.pos.y, 0);
}

/**
\brief Função que determina o movimento do cavaleiro.
@param e Estado do jogo.
@param n Índice do inimigo no array de inimigos do estado.
@param m_guerreiro Matriz de movimento.
*/
void mover_cavaleiro(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    for (i = inimigo->pos.x - 2; i <= inimigo->pos.x + 2; i++) {
        for (j = inimigo->pos.y - 2; j <= inimigo->pos.y + 2; j++) {
            p.x = i;
            p.y = j;
            if (posicao_valida(p) &&
                !posicao_ocupada(*e, p) &&
                movimento_valido_cav(inimigo->pos.x - i, inimigo->pos.y - j)) {
                if ((!adjacente(inimigo->pos, e->jog.pos) &&
                    m_guerreiro[j][i] <= m_guerreiro[ny][nx]) ||
                    (adjacente(inimigo->pos, e->jog.pos) &&
                    adjacente(p, e->jog.pos))) {
                    nx = i;
                    ny = j;
                }
            }
        }
    }
    inimigo->pos.x = nx;
    inimigo->pos.y = ny;
}

/**
\brief Função que determina o movimento do guerreiro.
@param e Estado do jogo.
@param n Índice do inimigo no array de inimigos do estado.
@param m_guerreiro Matriz de movimento.
*/
void mover_guerreiro(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    if (!adjacente(inimigo->pos, e->jog.pos)) {
        for (i = inimigo->pos.x - 1; i <= inimigo->pos.x + 1; i++)
            for (j = inimigo->pos.y - 1; j <= inimigo->pos.y + 1; j++) {
                p.x = i;
                p.y = j;
                if (posicao_valida(p) &&
                    !posicao_ocupada(*e, p) &&
                    movimento_valido(inimigo->pos.x - i, inimigo->pos.y - j) &&
                    m_guerreiro[j][i] < m_guerreiro[ny][nx]) {
                    nx = i;
                    ny = j;
                }
            }
    }
    inimigo->pos.x = nx;
    inimigo->pos.y = ny;
}

/**
\brief Função que determina o movimento do corredor.
@param e Estado do jogo.
@param n Índice do inimigo no array de inimigos do estado.
@param m_guerreiro Matriz de movimento.
*/
void mover_corredor(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    for (i = inimigo->pos.x - 1; i <= inimigo->pos.x + 1; i++)
        for (j = inimigo->pos.y - 1; j <= inimigo->pos.y + 1; j++) {
            p.x = i;
            p.y = j;
            if (posicao_valida(p) &&
                !posicao_ocupada(*e, p) &&
                movimento_valido(inimigo->pos.x - i, inimigo->pos.y - j) &&
                m_guerreiro[j][i] < m_guerreiro[ny][nx]) {
                nx = i;
                ny = j;
            }
        }
    inimigo->pos.x = nx;
    inimigo->pos.y = ny;
}

/**
\brief Função que determina o movimento de um inimigo.
@param e Estado do jogo.
@param n Índice do inimigo no array de inimigos do estado.
@param m_guerreiro Matriz de movimento.
*/
void mover_inimigo(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    static void (*f[])(ESTADO *e, int n, int m_guerreio[TAM][TAM]) = {
        mover_guerreiro,
        mover_corredor,
        mover_cavaleiro
    };
    INIMIGO *inimigo = &(e->inimigo[n]);
    (f[inimigo->tipo])(e, n, m_guerreiro);
}

/**
\brief Função que determina quando um guerreiro ataca o jogador.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
@param i Índice do inimigo no array de inimigos do estado.
*/
void ataque_guerreiro(ESTADO *e, ESTADO antigo, int i)
{
    if (posicao_igual(e->inimigo[i].pos, antigo.inimigo[i].pos) &&
        adjacente(e->jog.pos, e->inimigo[i].pos))
        e->jog.vidas--;
}

/**
\brief Função que determina quando um corredor ataca o jogador.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
@param i Índice do inimigo no array de inimigos do estado.
*/
void ataque_corredor(ESTADO *e, ESTADO antigo, int i)
{
    if (colinear(e->inimigo[i].pos, antigo.inimigo[i].pos, e->jog.pos) &&
        adjacente(e->jog.pos, e->inimigo[i].pos))
        e->jog.vidas--;
}

/**
\brief Função que determina quando um cavaleiro ataca o jogador.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
@param i Índice do inimigo no array de inimigos do estado.
*/
void ataque_cavaleiro(ESTADO *e, ESTADO antigo, int i)
{
    if (!posicao_igual(e->inimigo[i].pos, antigo.inimigo[i].pos) &&
        adjacente(e->jog.pos, antigo.inimigo[i].pos) &&
        adjacente(e->jog.pos, e->inimigo[i].pos))
        e->jog.vidas--;
}

/**
\brief Função que determina o dano que o jogador recebe num turno.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
*/
void matar_jogador(ESTADO *e, ESTADO antigo)
{
    int i;
    static void (*f[])(ESTADO *e, ESTADO antigo, int i) = {
        ataque_guerreiro,
        ataque_corredor,
        ataque_cavaleiro
    };
    for (i = 0; i < e->num_inimigos; i++)
        (f[e->inimigo[i].tipo])(e, antigo, i);
    if (e->jog.vidas < 0)
        e->jog.vidas = 0;
}

/**
\brief Função que elimina um inimigo do array de inimigos do estado.
@param e Estado do jogo.
@param n Índice do inimigo no array de inimigos do estado.
*/
void eliminar_inimigo(ESTADO *e, int n)
{
    int i;
    for (i = n + 1; i <= e->num_inimigos; i++)
        e->inimigo[i - 1] = e->inimigo[i];
    e->num_inimigos--;
}

/**
\brief Função que determina uma posição nova a partir da antiga e um movimento.
@param antiga Posição antiga.
@param mov Movimento a aplicar.
@returns POSICAO Nova posição.
*/
POSICAO nova_posicao(POSICAO antiga, char mov)
{
    switch (mov) {
        case 'r':
            antiga.x++;
            break;
        case 'l':
            antiga.x--;
            break;
        case 'u':
            antiga.y--;
            break;
        case 'd':
            antiga.y++;
            break;
        case 'f':
            antiga.x++;
            antiga.y--;
            break;
        case 'b':
            antiga.x--;
            antiga.y++;
            break;
    }
    return antiga;
}

/**
\brief Função que atualiza o estado do jogo caso o modo atual seja o modo normal.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
@param mov Movimento a aplicar.
*/
void atualizar_normal(ESTADO *e, ESTADO *antigo, char mov)
{
    POSICAO nova_pos;
    int i;
    int adj, lunge;
    nova_pos = nova_posicao(e->jog.pos, mov);
    for (i = 0; i < e->num_inimigos; i++) {
        adj = adjacente(e->inimigo[i].pos, e->jog.pos);
        lunge = colinear(e->inimigo[i].pos, e->jog.pos, nova_pos);
        if (adjacente(e->inimigo[i].pos, nova_pos) && (adj || lunge)) {
            eliminar_inimigo(e, i);
            eliminar_inimigo(antigo, i);
            i--;
            e->jog.score++;
        }
    }
    e->jog.pos = nova_pos;
}

/**
\brief Função que atualiza o estado do jogo caso o modo atual seja o modo de ataque.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
@param mov Direção de ataque.
*/
void atualizar_ataque(ESTADO *e, ESTADO *antigo, char mov)
{
    int i;
    POSICAO pos_ataque = nova_posicao(e->jog.pos, mov);
    for(i = 0; i < e->num_inimigos; i++){
        if (posicao_igual(pos_ataque, e->inimigo[i].pos)) {
            eliminar_inimigo(e, i);
            eliminar_inimigo(antigo, i);
            e->jog.score++;
            break;
        }
    }
    e->jog.modo = 'n';
}

/**
\brief Função que atualiza a posição dos inimigos e se eles causam danos ao jogador.
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
*/
void mover_inimigos(ESTADO *e, ESTADO antigo)
{
    int i;
    int m_guerreiro[TAM][TAM];
    matriz_guerreiro(*e, m_guerreiro);
    for (i = 0; i < e->num_inimigos; i++) {
        mover_inimigo(e, i, m_guerreiro);
    }
    matar_jogador(e, antigo);
}

/**
\brief Função que atualiza o jogador (movimento e ataques).
@param e Estado do jogo.
@param antigo Antigo estado do jogo.
@param mov Direção da ação executada.
*/
void atualizar_jogador(ESTADO *e, ESTADO *antigo, char mov)
{
    switch (e->jog.modo) {
        case NORMAL:
            atualizar_normal(e, antigo, mov);
            break;
        case ATAQUE:
            atualizar_ataque(e, antigo, mov);
            break;
        default:
            break;
    }
}

/**
\brief Função que atualiza o estado.
@param e Estado do jogo.
@param query Argumento no query string.
@returns ESTADO Novo estado.
*/
ESTADO atualizar_estado(ESTADO e, char query[])
{
    ESTADO antigo = e;
    int pagina;
    char modo, mov;
    sscanf(query, "%d", &pagina);
    if (pagina == 1) {
        sscanf(query, "1,%c", &modo);
        if (e.jog.vidas <= 0 || modo == 'x')
            return e;
        sscanf(query, "1,%c,%c", (char *)&e.jog.modo, &mov);
        atualizar_jogador(&e, &antigo, mov);

        if (e.jog.modo != MUDAR_MODO)
            mover_inimigos(&e, antigo);
        else
            e.jog.modo = mov;
        return e;

    } else {
        return inicializar(INIT_VIDAS, 0);
    }
}

