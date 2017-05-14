#include <stdlib.h>
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

void imprime_vida(int x)
{
    int y = 7;
    IMAGEM(x, y, OFFSET_WIDTH, OFFSET_HEIGHT, ESCALA, "Heart2.png");
}

void imprime_vidas(ESTADO e)
{
    int i;
    for(i = 0; i < e.jog.vidas; i++){
        imprime_vida(i);
    }
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
    POSICAO p;
    for (y = 0; y < TAM; y++) {
        p.y = y;
        for (x = 0; x < TAM; x++) {
            p.x = x;
            if (posicao_valida(p))
                imprime_casa(e, p);
        }
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
    if (e.jog.vidas > 0) {
        p.x = e.jog.pos.x + dx;
        p.y = e.jog.pos.y + dy;
        if (!posicao_valida(p) || posicao_ocupada(e, p)) return;
        if (tem_saida(e, p)) {
            sprintf(link, "http://localhost/cgi-bin/main?x,%d,%d", e.jog.vidas, e.jog.score);
            ABRIR_LINK(link);
            imprime_casa(e, p);
            FECHAR_LINK;
            return;
        }
        mov = determinar_mov(dx, dy);
        sprintf(link, "http://localhost/cgi-bin/main?n,%c", mov);
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

void imprime_ataque(ESTADO e, int dx, int dy)
{
    char mov;
    char link[MAX_BUFFER];
    POSICAO p;
    if (e.jog.vidas > 0) {
        p.x = e.jog.pos.x + dx;
        p.y = e.jog.pos.y + dy;
        if (posicao_valida(p) && tem_inimigo(e, p)) {
            mov = determinar_mov(dx, dy);
            sprintf(link, "http://localhost/cgi-bin/main?a,%c", mov);
            ABRIR_LINK(link);
            imprime_casa(e, p);
            FECHAR_LINK;
        }
    }
}

void imprime_ataques(ESTADO e)
{
    imprime_ataque(e,  0, -1);
    imprime_ataque(e,  0, +1);
    imprime_ataque(e, -1,  0);
    imprime_ataque(e, +1,  0);
    imprime_ataque(e, -1, +1);
    imprime_ataque(e, +1, -1);
}

void imprime_jogador(ESTADO e)
{
    int x_offset = calc_xoffset(e.jog.pos.y);
    int y_offset = calc_yoffset(e.jog.pos.y);
    if (e.jog.vidas > 0) {
        IMAGEM(e.jog.pos.x, e.jog.pos.y, x_offset, y_offset, ESCALA, "DwellerN_03.png");
        switch (e.jog.modo) {
            case NORMAL:
                imprime_movimentos(e);
                break;
            case ATAQUE:
                imprime_ataques(e);
                break;
            default:
                break;
        }
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

void imprime_modo(char modo, int x)
{
    int y = 8;
    char link[MAX_BUFFER];
    sprintf(link, "http://localhost/cgi-bin/main?m,%c", modo);
    ABRIR_LINK(link);
    IMAGEM(x, y, OFFSET_WIDTH, OFFSET_HEIGHT, ESCALA, "Heart2.png");
    FECHAR_LINK;
}

void imprime_modos(ESTADO e)
{
    char modos[] = {'n', 'a', '\0'};
    int i;
    for (i = 0; modos[i] != '\0'; i++) {
        if (modos[i] != (char)e.jog.modo)
            imprime_modo(modos[i], i);
    }
}

void imprime_jogo(ESTADO e)
{
    imprime_tabuleiro(e);
    imprime_vidas(e);
    imprime_inimigos(e);
    imprime_jogador(e);
    imprime_obstaculos(e);
    imprime_modos(e);
}

void imprime_top(char top_scores[])
{
    FILE *file = fopen(top_scores, "r");
    int i, x;
    /* char buf[102040]; */
    if(file == NULL) {
            perror("nao consegui abrir o ficheiro de scores para leitura");
            exit(1);
    }
    /* fprintf(stderr, "lendo a linha: %p\n", fgets(buf, 10240, file));
    fprintf(stderr, "primeira linha: %s\n", buf);
    fprintf(stderr, "<!-- %p %d -->\n", file, fscanf(file, "%d\n", &x)); */
    rewind(file);
    ABRIR_TABELA;
    for (i = 0; fscanf(file, "%d\n", &x) && i < 10; i++) {
        ABRIR_LINHA;
        IMPRIMIR_CELULA(x);
        FECHAR_LINHA;
    }
    FECHAR_TABELA;
    fclose(file);
}
