#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "estado.h"

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

int posicao_valida(int x, int y)
{
    return x >= 0 && y >= 0 && x < TAM && y < TAM;
}

int posicao_igual(POSICAO p, int x, int y)
{
    return p.x == x && p.y == y;
}

int tem_jogador(ESTADO e, int x, int y)
{
    return posicao_igual(e.jog, x, y);
}

int tem_inimigo(ESTADO e, int x, int y)
{
    int i;
    for (i = 0; i < e.num_inimigos; i++) {
        if (posicao_igual(e.inimigo[i], x, y))
            return 1;
        }
    return 0;
}

int tem_obstaculo(ESTADO e, int x, int y)
{
    int i;
    for (i = 0; i < e.num_obstaculos; i++) {
        if (posicao_igual(e.obstaculo[i], x, y))
            return 1;
    }
    return 0;
}

int tem_saida(ESTADO e, int x, int y)
{
    return posicao_igual(e.saida, x, y);
}

int posicao_ocupada(ESTADO e, int x, int y)
{
    return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y);
}

ESTADO inicializar_inimigo(ESTADO e) {
    int x, y;

    do {
        x = rand() % TAM;
        y = rand() % TAM;
    } while (posicao_ocupada(e, x, y) || tem_saida(e, x, y));

    e.inimigo[(int)e.num_inimigos].x = x;
    e.inimigo[(int)e.num_inimigos].y = y;
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
    int x, y;

    do {
        x = rand() % TAM;
        y = rand() % TAM;
    } while(posicao_ocupada(e, x, y) || tem_saida(e, x, y));

    e.obstaculo[(int)e.num_obstaculos].x = x;
    e.obstaculo[(int)e.num_obstaculos].y = y;
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
    int x, y;
    ESTADO e = {{0,0},0,0,{{0}},{{0}},{0,0}};
    do {
        x = rand() % TAM;
        y = rand() % TAM;
    } while (posicao_ocupada(e, x, y));
    e.jog.x = x;
    e.jog.y = y;
    do {
        x = rand() % TAM;
        y = rand() % TAM;
    } while (posicao_ocupada(e, x, y));
    e.saida.x = x;
    e.saida.y = y;
    e = inicializar_inimigos(e, 20);
    e = inicializar_obstaculos(e, 20);
    return e;
}

ESTADO ler_estado(char *args)
{
    if(strlen(args) == 0)
        return inicializar();
    return str2estado(args);
}

int adjacente(POSICAO p1, POSICAO p2)
{
    return (abs(p1.x - p2.x) < 2) &&
           (abs(p1.y - p2.y) < 2) &&
           (abs(get_z(p1) - get_z(p2)) < 2);
}

int get_z(POSICAO p)
{
    return -(p.x + p.y);
}

int colinear(POSICAO p1, POSICAO p2, POSICAO p3)
{
    return (p1.x == p2.x && p2.x == p3.x) ||
           (p1.y == p2.y && p2.y == p3.y) ||
           (get_z(p1) == get_z(p2) && get_z(p2) == get_z(p3));
}

void eliminar_inimigo(ESTADO *e, int n)
{
    int i;
    for (i = n + 1; i <= e->num_inimigos; i++)
        e->inimigo[i - 1] = e->inimigo[i];
    e->num_inimigos--;
}

ESTADO atualizar_estado(ESTADO e, int x, int y)
{
    ESTADO novo;
    int i;
    int adj, lunge;
    POSICAO nova_pos = {x, y};
    novo = e;
    for (i = 0; i < e.num_inimigos; i++) {
        adj = adjacente(e.inimigo[i], e.jog);
        lunge = colinear(e.inimigo[i], e.jog, nova_pos);
        if (adjacente(e.inimigo[i], nova_pos) && (adj || lunge)) {
            eliminar_inimigo(&e, i);
            i--;
        }
        /* e.inimigo[i].x++; */
    }
    e.jog = nova_pos;
    novo = e;
    return novo;
}

