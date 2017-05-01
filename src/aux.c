#include <math.h>
#include <stdlib.h>

#include "estado.h"

int get_z(POSICAO p)
{
    return -(p.x + p.y);
}

int posicao_valida(int x, int y)
{
    POSICAO pos, center_pos;
    pos.x = x;
    pos.y = y;
    center_pos.x = TAM / 2;
    center_pos.y = TAM / 2;
    return abs(x - TAM / 2) <= TAM / 2 &&
           abs(y - TAM / 2) <= TAM / 2 &&
           abs(get_z(pos) - get_z(center_pos)) <= TAM / 2;
}

int movimento_valido(int dx, int dy)
{
    POSICAO p;
    p.x = dx; 
    p.y = dy; 
    return abs(dx)            < 2 &&
           abs(dy)            < 2 &&
           abs(get_z(p)) < 2;
}

void rand_pos(ESTADO e, int *x, int *y, int testar_saida)
{
    do {
        *x = rand() % TAM;
        *y = rand() % TAM;
    } while (!posicao_valida(*x, *y) || posicao_ocupada(e, *x, *y) || (tem_saida(e, *x, *y) && testar_saida));
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
        if (posicao_igual(e.inimigo[i].pos, x, y))
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

int adjacente(POSICAO p1, POSICAO p2)
{
    return (abs(p1.x - p2.x) < 2) &&
           (abs(p1.y - p2.y) < 2) &&
           (abs(get_z(p1) - get_z(p2)) < 2);
}

int colinear(POSICAO p1, POSICAO p2, POSICAO p3)
{
    return (p1.x == p2.x && p2.x == p3.x) ||
           (p1.y == p2.y && p2.y == p3.y) ||
           (get_z(p1) == get_z(p2) && get_z(p2) == get_z(p3));
}

