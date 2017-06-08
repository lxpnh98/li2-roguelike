#include <stdlib.h>
#include <math.h>
#include <string.h>

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
    IMAGEM(x, y, OFFSET_WIDTH, OFFSET_HEIGHT, ESCALA, "heart.png");
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
    IMAGEM(p.x, p.y, x_offset, y_offset, ESCALA, "basic_hex.png");
    if (tem_saida(e, p))
        IMAGEM(p.x, p.y, x_offset, y_offset, ESCALA, "hexit.png");
}

void imprime_casa_transparente(POSICAO p)
{
    int x_offset = calc_xoffset(p.y);
    int y_offset = calc_yoffset(p.y);
    IMAGEM(p.x, p.y, x_offset, y_offset, ESCALA, "alpha_hex.png");
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
            sprintf(link, "http://localhost/cgi-bin/main?1,x,%d,%d", e.jog.vidas, e.jog.score);
            ABRIR_LINK(link);
            imprime_casa(e, p);
            FECHAR_LINK;
            return;
        }
        mov = determinar_mov(dx, dy);
        sprintf(link, "http://localhost/cgi-bin/main?1,n,%c", mov);
        ABRIR_LINK(link);
        imprime_casa_transparente(p);
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
            sprintf(link, "http://localhost/cgi-bin/main?1,a,%c", mov);
            ABRIR_LINK(link);
            imprime_casa_transparente(p);
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
        IMAGEM(e.jog.pos.x, e.jog.pos.y, x_offset, y_offset, ESCALA, "player.png");

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
        switch (e.inimigo[i].tipo) {
            case GUERREIRO:
                IMAGEM(e.inimigo[i].pos.x, e.inimigo[i].pos.y, x_offset, y_offset, ESCALA, "guerreiro.png");
                break;
            case CORREDOR:
                IMAGEM(e.inimigo[i].pos.x, e.inimigo[i].pos.y, x_offset, y_offset, ESCALA, "corredor.png");
                break;
            case CAVALEIRO:
                IMAGEM(e.inimigo[i].pos.x, e.inimigo[i].pos.y, x_offset, y_offset, ESCALA, "cavaleiro.png");
                break;
        }
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
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, x_offset, y_offset, ESCALA, "obstaculo.png");
    }
}

void imprime_imagem(int x, int y, int x_offset, int y_offset, int escala, char ficheiro[])
{
    char link[MAX_BUFFER];
    sprintf(link, "%s%s", IMAGE_PATH, ficheiro);
    printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n",
        escala * x + x_offset, escala * y + y_offset, escala, escala, link);
}

void imprime_modo(char modo, int x)
{
    static char modos[] = "na";
    static char *imgs[] = {"botao_normal.png", "botao_ataque.png"};
    int y = 8;
    char link[MAX_BUFFER];
    sprintf(link, "http://localhost/cgi-bin/main?1,m,%c", modo);
    ABRIR_LINK(link);
    int i;
    for (i = 0; modos[i] != '\0'; i++)
        if (modos[i] == modo)
            imprime_imagem(x, y, OFFSET_WIDTH, OFFSET_HEIGHT, ESCALA, imgs[i]);
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

/**
    \brief Função que imprime no ecrâ o top10 das melhores pontuções do jogador

    @param não sei o que por aqui
*/
void imprime_top(char top_scores[])
{
    FILE *file = fopen(top_scores, "r");
    int i, x;
    if(file == NULL) {
            perror("nao consegui abrir o ficheiro de scores para leitura");
            exit(1);
    }
    rewind(file);
    ABRIR_TABELA(20, 50);
    ABRIR_LINHA;
    IMPRIMIR_CABECALHO("Ranking");
    IMPRIMIR_CABECALHO("Score");
    FECHAR_LINHA;
    for (i = 0; fscanf(file, "%d\n", &x) && i < 10; i++) {
        ABRIR_LINHA;
        IMPRIMIR_CELULA(i+1);
        IMPRIMIR_CELULA(x);
        FECHAR_LINHA;
    }
    FECHAR_TABELA;
    fclose(file);
}

/**
    \brief Função que imprime no ecrâ o botão que inicia o novo jogo.
*/
void imprime_retorno()
{
    printf("<a href=\"http://localhost/cgi-bin/main?0\" class=\"button\">Novo jogo</a>");
}

void guardar_pontuacao(char top_scores[], JOGADOR jog)
{
    int i, j;
    int scores[10];
    FILE *score_file = fopen(top_scores, "r");
    if(score_file == NULL) {
        fprintf(stderr, "nao consegui abrir o ficheiro de scores para leitura");
        exit(1);
    }

    /* Carregar pontuações mais altas, preenchendo o resto da array com 0s. */
    for (i = 0; fscanf(score_file, "%d\n", &scores[i]) == 1 && i < 10; i++);
    for (; i < 10; i++) scores[i] = 0;

    /* Inserir pontuação na array ordenada. */
    for (i = 0; i < 10; i++) {
        if (jog.score > scores[i]) {
            for (j = 9; j > i; j--)
                scores[j] = scores[j-1];
            scores[j] = jog.score;
            break;
        }
    }
    fclose(score_file);

    /* Escrever o top das pontuações atualizado para o ficheiro. */
    score_file = fopen(top_scores, "w+");
    for (i = 0; i < 10; i++) {
        fprintf(score_file, "%d\n", scores[i]);
    }
    rewind(score_file); /* Esta linha resove o problema inteiro. (elimina este comentário) */
    fclose(score_file);
}

void imprime_estado(ESTADO e)
{
    COMECAR_HTML;
    if (e.jog.vidas > 0) {
        ABRIR_SVG(600, 600);
        imprime_jogo(e);
        FECHAR_SVG;
    } else {
        guardar_pontuacao("/var/www/scores", e.jog);
        imprime_top("/var/www/scores");
        imprime_retorno();
    }
}
