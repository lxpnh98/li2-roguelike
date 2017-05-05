#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "estado.h"
#include "calc.h"

char *estado2str(ESTADO e)
{
    static char buffer[MAX_BUFFER];
    char *p = (char *) &e;
    int i;

    buffer[0] = 0;

    for(i = 0; (unsigned int)i < sizeof(ESTADO); i++)
        sprintf(buffer, "%s%02x", buffer, p[i]);
    
    return buffer;
}

ESTADO str2estado(char *argumentos)
{
    ESTADO e;
    char *p = (char *) &e;
    int i;

    for(i = 0; (unsigned int)i < sizeof(ESTADO); i++, argumentos += 2) {
        int d;
        sscanf(argumentos, "%2x", (unsigned int *)&d);
        p[i] = (char) d;
    }
    
    return e;
}

ESTADO inicializar_inimigo(ESTADO e) {
    POSICAO p;
    rand_pos(e, &p, 1);
    e.inimigo[(int)e.num_inimigos].tipo = GUERREIRO;
    e.inimigo[(int)e.num_inimigos].pos = p;
    e.num_inimigos++;

    return e;
}

ESTADO inicializar_inimigos(ESTADO e, int num) {
    int i;
    for (i = 0; i < num; i++)
        e = inicializar_inimigo(e);
    return e;
}

ESTADO inicializar_obstaculo(ESTADO e) {
    POSICAO p;
    rand_pos(e, &p, 1);
    e.obstaculo[(int)e.num_obstaculos] = p;
    e.num_obstaculos++;

    return e;
}

ESTADO inicializar_obstaculos(ESTADO e, int num) {
    int i;
    for (i = 0; i < num; i++)
        e = inicializar_obstaculo(e);
    return e;
}

ESTADO inicializar() {
    POSICAO p;
    ESTADO e = {{0,0},0,0,0,{{0}},{{0}},{0,0}};
    rand_pos(e, &p, 0);
    e.jog = p;
    e.vidas = 5;
    rand_pos(e, &p, 0);
    e.saida = p;
    e = inicializar_inimigos(e, 10);
    e = inicializar_obstaculos(e, 10);
    return e;
}

ESTADO ler_estado(FILE *file, char query[])
{
    char *e;
    int tamanho;

    /* Se não existir ficheiro ou o jogador entrar num novo nível, criar estado
     * aleatório. */
    if (file == NULL || query[0] == 'x')
        return inicializar();

    /* Descubrir tamanho do ficheiro. */
    fseek(file, 0, SEEK_END);
    tamanho = ftell(file);
    rewind(file);

    /* Se ficheiro existir mas for vazio, criar estado aleatório. */
    if (tamanho == 0)
        return inicializar();

    /* Alocar memória necessária, copiar conteúdos do ficheiro para a variável
     * e terminar o string. */
    e = (char *)malloc(sizeof(char) * (tamanho + 1));
    tamanho = fread(e, sizeof(char), tamanho, file);
    e[tamanho] = '\0';

    /* Converter o string para estado e retornar. */
    return str2estado(e);
}

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

void matriz_guerreiro(ESTADO e, int m[TAM][TAM])
{
    int i, j;
    for (i = 0; i < TAM; i++)
        for (j = 0; j < TAM; j++)
            m[i][j] = 999;
    preencher(m, e, e.jog.x, e.jog.y, 0);
}

void mover_guerreiro(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    if (!adjacente(inimigo->pos, e->jog)) {
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

void mover_inimigo(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    INIMIGO *inimigo = &(e->inimigo[n]);
    switch (inimigo->tipo) {
        case GUERREIRO:
            mover_guerreiro(e, n, m_guerreiro);
            break;
        case CORREDOR:
            mover_corredor(e, n, m_guerreiro);
            break;
    }
}

void matar_jogador(ESTADO *e, ESTADO antigo)
{
    int i;
    for (i = 0; i < e->num_inimigos; i++) {
        switch (e->inimigo[i].tipo) {
            case GUERREIRO:
                if (posicao_igual(e->inimigo[i].pos, antigo.inimigo[i].pos) &&
                    adjacente(e->jog, e->inimigo[i].pos))
                    e->vidas--;
                break;
            case CORREDOR:
                if (colinear(e->inimigo[i].pos, antigo.inimigo[i].pos, e->jog) &&
                    adjacente(e->jog, e->inimigo[i].pos))
                    e->vidas--;
                break;
        }
    }
    if (e->vidas < 0)
        e->vidas = 0;
}

void eliminar_inimigo(ESTADO *e, int n)
{
    int i;
    for (i = n + 1; i <= e->num_inimigos; i++)
        e->inimigo[i - 1] = e->inimigo[i];
    e->num_inimigos--;
}

ESTADO atualizar_estado(ESTADO e, char query[])
{
    ESTADO antigo = e;
    int i;
    int adj, lunge;
    int m_guerreiro[TAM][TAM];
    POSICAO nova_pos;
    nova_pos.x = e.jog.x;
    nova_pos.y = e.jog.y;
    switch (query[0]) {
        case 'r':
            nova_pos.x = e.jog.x + 1;
            nova_pos.y = e.jog.y;
            break;
        case 'l':
            nova_pos.x = e.jog.x - 1;
            nova_pos.y = e.jog.y;
            break;
        case 'u':
            nova_pos.x = e.jog.x;
            nova_pos.y = e.jog.y - 1;
            break;
        case 'd':
            nova_pos.x = e.jog.x;
            nova_pos.y = e.jog.y + 1;
            break;
        case 'f':
            nova_pos.x = e.jog.x + 1;
            nova_pos.y = e.jog.y - 1;
            break;
        case 'b':
            nova_pos.x = e.jog.x - 1;
            nova_pos.y = e.jog.y + 1;
            break;
    }
    for (i = 0; i < e.num_inimigos; i++) {
        adj = adjacente(e.inimigo[i].pos, e.jog);
        lunge = colinear(e.inimigo[i].pos, e.jog, nova_pos);
        if (adjacente(e.inimigo[i].pos, nova_pos) && (adj || lunge)) {
            eliminar_inimigo(&e, i);
            eliminar_inimigo(&antigo, i);
            i--;
        }
    }
    e.jog = nova_pos;

    /* Mover inimigos aqui */
    matriz_guerreiro(e, m_guerreiro);
    for (i = 0; i < e.num_inimigos; i++) {
        mover_inimigo(&e, i, m_guerreiro);
    }

    matar_jogador(&e, antigo);

    return e;
}

