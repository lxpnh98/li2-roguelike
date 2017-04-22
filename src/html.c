#include <math.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

#define OFFSET_WIDTH            20
#define OFFSET_HEIGHT           10

int calc_xoffset(int x, int y)
{
    return OFFSET_WIDTH * y;
}

int calc_yoffset(int y)
{
    return - OFFSET_HEIGHT * y;
}

void imprime_casa(ESTADO e, int x, int y)
{
    int x_offset = calc_xoffset(x, y);
    int y_offset = calc_yoffset(y);
    if (tem_saida(e, x, y))
        IMAGEM(x, y, x_offset, y_offset, ESCALA, "hexit.png");
    IMAGEM(x, y, x_offset, y_offset, ESCALA, "basic_hex.png");
}

void imprime_tabuleiro(ESTADO e)
{
    int x, y;
    for(y = 0; y < 10; y++) {
        for(x = 0; x < 10; x++) {
            imprime_casa(e, x, y);
        }
    }

    
}

void imprime_movimento(ESTADO e, int dx, int dy)
{
    ESTADO novo;
    int x = e.jog.x + dx;
    int y = e.jog.y + dy;
    char link[MAX_BUFFER];

    if (!posicao_valida(x, y) || posicao_ocupada(e, x, y)) return;
    if (tem_saida(e, x, y)) {
        sprintf(link, "http://localhost/cgi-bin/main");
        ABRIR_LINK(link);
        imprime_casa(e, x, y);
        FECHAR_LINK;
        return;
    }
    novo = atualizar_estado(e, x, y);
    sprintf(link, "http://localhost/cgi-bin/main?%s", estado2str(novo));
    ABRIR_LINK(link);
    imprime_casa(e, x, y);
    FECHAR_LINK;
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
    int x_offset = calc_xoffset(e.jog.x, e.jog.y);
    int y_offset = calc_yoffset(e.jog.y);
    IMAGEM(e.jog.x, e.jog.y, x_offset, y_offset, ESCALA, "DwellerN_03.png");
    imprime_movimentos(e);
}

void imprime_inimigos(ESTADO e)
{
    int x_offset = 0;
    int y_offset = 0;
    int i;
    for(i = 0; i < e.num_inimigos; i++) {
        x_offset = calc_xoffset(e.inimigo[i].x, e.inimigo[i].y);
        y_offset = calc_yoffset(e.inimigo[i].y);
        IMAGEM(e.inimigo[i].x, e.inimigo[i].y, x_offset, y_offset, ESCALA, "ks01-ogre_mage2_02_hi.png");
    }
}

void imprime_obstaculos(ESTADO e)
{
    int x_offset = 0;
    int y_offset = 0;
    int i;
    for(i = 0; i < e.num_obstaculos; i++) {
        x_offset = calc_xoffset(e.obstaculo[i].x, e.obstaculo[i].y);
        y_offset = calc_yoffset(e.obstaculo[i].y);
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, x_offset, y_offset, ESCALA, "lava_pool1.png");
    }
}
