#include "cgi.h"
#include "estado.h"
#include "html.h"

void imprime_casa(ESTADO e, int x, int y)
{
    char *cor[] = {"#4d4d33", "#d6d6c2", "#11aa22"};
    int idx = (tem_saida(e, x, y) ? 2 : (x + y) % 2);
    QUADRADO(x, y,ESCALA, cor[idx]);
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
    imprime_movimento(e, -1, -1);
    imprime_movimento(e, +1, +1);
    imprime_movimento(e, -1, +1);
    imprime_movimento(e, +1, -1);
}

void imprime_jogador(ESTADO e)
{
    IMAGEM(e.jog.x, e.jog.y, ESCALA, "DwellerN_03.png");
    imprime_movimentos(e);
}

void imprime_inimigos(ESTADO e)
{
    int i;
    for(i = 0; i < e.num_inimigos; i++)
        IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "ks01-ogre_mage2_02_hi.png");
}

void imprime_obstaculos(ESTADO e)
{
    int i;
    for(i = 0; i < e.num_obstaculos; i++)
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
}
