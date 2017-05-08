#include <math.h>

#include "cgi.h"
#include "estado.h"
#include "calc.h"
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

void imprime_vidas(int x)
{
    int y = 7;
    IMAGEM(x, y, OFFSET_WIDTH, OFFSET_HEIGHT, ESCALA, "Heart2.png");
}

void imprime_casa(ESTADO e, POSICAO p)
{
    int x_offset = calc_xoffset(p.y);
    int y_offset = calc_yoffset(p.y);
    if (tem_saida(e, p))
        IMAGEM(p.x, p.y, x_offset, y_offset, ESCALA, "hexit.png");
    IMAGEM(p.x, p.y, x_offset, y_offset, ESCALA, "basic_hex.png");
}

void imprime_tabuleiro(ESTADO e)
{
    int x, y;
    int nvidas = e.vidas;
    POSICAO p;
    for (y = 0; y < TAM; y++) {
        p.y = y;
        for (x = 0; x < TAM; x++) {
            p.x = x;
            if (posicao_valida(p))
                imprime_casa(e, p);
        }
    }
    for(x = -1; nvidas > 0; nvidas--){
        x++;
        imprime_vidas(x);
    }
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
    POSICAO p;
    if (e.vidas > 0) {
        p.x = e.jog.x + dx;
        p.y = e.jog.y + dy;
        if (!posicao_valida(p) || posicao_ocupada(e, p)) return;
        if (tem_saida(e, p)) {
            sprintf(link, "http://localhost/cgi-bin/main?x");
            ABRIR_LINK(link);
            imprime_casa(e, p);
            FECHAR_LINK;
            return;
        }
        mov = determinar_mov(dx, dy);
        sprintf(link, "http://localhost/cgi-bin/main?%c", mov);
        ABRIR_LINK(link);
        imprime_casa(e, p);
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

void imprime_jogo(ESTADO e)
{
    imprime_tabuleiro(e);
    imprime_jogador(e);
    imprime_inimigos(e);
    imprime_obstaculos(e);
}

void imprime_top()
{}
