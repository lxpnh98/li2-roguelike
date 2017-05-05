#include <math.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

#define OFFSET_WIDTH            20
#define OFFSET_HEIGHT           10

#define TAM_CASA                45

int calc_xoffset(int y)
{
    return OFFSET_WIDTH * y;
}

int calc_yoffset(int y)
{
    return - OFFSET_HEIGHT * y;
}

void imprime_casa(ESTADO e, int x, int y)
{
    int x_offset = calc_xoffset(y);
    int y_offset = calc_yoffset(y);
    if (tem_saida(e, x, y))
        IMAGEM(x, y, x_offset, y_offset, ESCALA, "hexit.png");
    IMAGEM(x, y, x_offset, y_offset, ESCALA, "basic_hex.png");
}

void imprime_tabuleiro(ESTADO e)
{
    int x, y;
    for (y = 0; y < TAM; y++)
        for (x = 0; x < TAM; x++)
            if (posicao_valida(x, y))
                imprime_casa(e, x, y);
}

char determinar_mov(int dx, int dy)
{
    int mov;
    if      (dx ==  0 && dy == +1) mov = 'd';
    else if (dx ==  0 && dy == -1) mov = 'u';
    else if (dx == +1 && dy ==  0) mov = 'r';
    else if (dx == -1 && dy ==  0) mov = 'l';
    else if (dx == +1 && dy == -1) mov = 'f';
    else                           mov = 'b';
    return mov;
}

void imprime_movimento(ESTADO e, int dx, int dy)
{
    char mov;
    char link[MAX_BUFFER];
    int x, y;
    if (e.vidas > 0) {
        x = e.jog.x + dx;
        y = e.jog.y + dy;
        if (!posicao_valida(x, y) || posicao_ocupada(e, x, y)) return;
        if (tem_saida(e, x, y)) {
            sprintf(link, "http://localhost/cgi-bin/main?x");
            ABRIR_LINK(link);
            imprime_casa(e, x, y);
            FECHAR_LINK;
            return;
        }
        mov = determinar_mov(dx, dy);
        sprintf(link, "http://localhost/cgi-bin/main?%c", mov);
        ABRIR_LINK(link);
        imprime_casa(e, x, y);
        FECHAR_LINK;
    }
}

void imprime_movimentos(ESTADO e)
{
    imprime_movimento(e,  0, -1);
    imprime_movimento(e,  0, +1);
    imprime_movimento(e, -1,  0);
    imprime_movimento(e, +1,  0);
    imprime_movimento(e, -1, +1);
    imprime_movimento(e, +1, -1);
}

void imprime_jogador(ESTADO e)
{
    int x_offset = calc_xoffset(e.jog.y);
    int y_offset = calc_yoffset(e.jog.y);
    if (e.vidas > 0) {
        IMAGEM(e.jog.x, e.jog.y, x_offset, y_offset, ESCALA, "DwellerN_03.png");
        imprime_movimentos(e);
    }
}

void imprime_inimigos(ESTADO e)
{
    int x_offset = 0;
    int y_offset = 0;
    int i;
    for(i = 0; i < e.num_inimigos; i++) {
        x_offset = calc_xoffset(e.inimigo[i].pos.y);
        y_offset = calc_yoffset(e.inimigo[i].pos.y);
        IMAGEM(e.inimigo[i].pos.x, e.inimigo[i].pos.y, x_offset, y_offset, ESCALA, "ks01-ogre_mage2_02_hi.png");
    }
}

void imprime_obstaculos(ESTADO e)
{
    int x_offset = 0;
    int y_offset = 0;
    int i;
    for(i = 0; i < e.num_obstaculos; i++) {
        x_offset = calc_xoffset(e.obstaculo[i].y);
        y_offset = calc_yoffset(e.obstaculo[i].y);
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, x_offset, y_offset, ESCALA, "lava_pool1.png");
    }
}
