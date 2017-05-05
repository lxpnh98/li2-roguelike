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

void rand_pos(ESTADO e, POSICAO *p, int testar_saida)
{
    do {
        p->x = rand() % TAM;
        p->y = rand() % TAM;
    } while (!posicao_valida(*p) || posicao_ocupada(e, *p) || (tem_saida(e, *p) && testar_saida));
}

int posicao_igual(POSICAO p1, POSICAO p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

int tem_jogador(ESTADO e, POSICAO p)
{
    return posicao_igual(e.jog, p);
}

int tem_inimigo(ESTADO e, POSICAO p)
{
    int i;
    for (i = 0; i < e.num_inimigos; i++) {
        if (posicao_igual(e.inimigo[i].pos, p))
            return 1;
        }
    return 0;
}

int tem_obstaculo(ESTADO e, POSICAO p)
{
    int i;
    for (i = 0; i < e.num_obstaculos; i++) {
        if (posicao_igual(e.obstaculo[i], p))
            return 1;
    }
    return 0;
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

