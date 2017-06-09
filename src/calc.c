/**
@file calc.c
Ficheiro contendo funções auxiliares à inicialização e atualização do estado.
*/

#include <math.h>
#include <stdlib.h>

#include "estado.h"
#include "calc.h"

int get_z(POSICAO p)
{
    return -(p.x + p.y);
}

int posicao_valida(POSICAO p)
{
    POSICAO center_pos;
    center_pos.x = TAM / 2;
    center_pos.y = TAM / 2;
    return abs(p.x - TAM / 2)                <= TAM / 2 &&
           abs(p.y - TAM / 2)                <= TAM / 2 &&
           abs(get_z(p) - get_z(center_pos)) <= TAM / 2;
}

int movimento_valido(int dx, int dy)
{
    POSICAO p;
    p.x = dx; 
    p.y = dy; 
    return abs(dx)       < 2 &&
           abs(dy)       < 2 &&
           abs(get_z(p)) < 2;
}

int movimento_valido_cav(int dx, int dy)
{
    return (dx == +2 && dy ==  0) ||
           (dx == -2 && dy ==  0) ||
           (dx ==  0 && dy == -2) ||
           (dx ==  0 && dy == +2) ||
           (dx == +2 && dy == -2) ||
           (dx == -2 && dy == +2) ||
           (dx == +1 && dy == -2) ||
           (dx == -1 && dy == +2) ||
           (dx == +2 && dy == -1) ||
           (dx == -2 && dy == +1) ||
           (dx == +1 && dy == +1) ||
           (dx == -1 && dy == -1);
}

void rand_pos(ESTADO e, POSICAO *p, int testar_saida)
{
    do {
        p->x = rand() % TAM;
        p->y = rand() % TAM;
    } while (!posicao_valida(*p)    ||
             posicao_ocupada(e, *p) ||
             (tem_saida(e, *p) && testar_saida));
}

int posicao_igual(POSICAO p1, POSICAO p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

int tem_jogador(ESTADO e, POSICAO p)
{
    return posicao_igual(e.jog.pos, p);
}

int tem_inimigo(ESTADO e, POSICAO p)
{
    int i, r = 0;
    for (i = 0; i < e.num_inimigos; i++) {
        if (posicao_igual(e.inimigo[i].pos, p)) {
            r = 1;
            break;
        }
    }
    return r;
}

int tem_obstaculo(ESTADO e, POSICAO p)
{
    int i, r = 0;
    for (i = 0; i < e.num_obstaculos; i++) {
        if (posicao_igual(e.obstaculo[i], p)) {
            r = 1;
            break;
        }
    }
    return r;
}

int tem_saida(ESTADO e, POSICAO p)
{
    return posicao_igual(e.saida, p);
}

int posicao_ocupada(ESTADO e, POSICAO p)
{
    return tem_jogador(e, p) ||
           tem_inimigo(e, p) ||
           tem_obstaculo(e, p);
}

int adjacente(POSICAO p1, POSICAO p2)
{
    return (abs(p1.x - p2.x)           < 2) &&
           (abs(p1.y - p2.y)           < 2) &&
           (abs(get_z(p1) - get_z(p2)) < 2);
}

int colinear(POSICAO p1, POSICAO p2, POSICAO p3)
{
    return (p1.x == p2.x && p2.x == p3.x) ||
           (p1.y == p2.y && p2.y == p3.y) ||
           (get_z(p1) == get_z(p2) && get_z(p2) == get_z(p3));
}

