#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"

#define MAX_BUFFER        10240
#define TAM                10
#define ESCALA            40

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
    return tem_jogador(e, x, y) || tem_inimigo(e, x, y) || tem_obstaculo(e, x, y); // || tem_saida(e, x, y);
}

void imprime_casa(ESTADO e, int x, int y)
{
    char *cor[] = {"#666600", "#555500", "#338855"};
    int idx = (tem_saida(e, x, y) ? 2 : (x + y) % 2);
    QUADRADO(x, y,ESCALA, cor[idx]);
}    

ESTADO inicializar_inimigo(ESTADO e) {
    int x, y;

    do {
        x = random() % TAM;
        y = random() % TAM;
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
        x = random() % TAM;
        y = random() % TAM;
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
    ESTADO e = {{0}};
    do {
        x = random() % TAM;
        y = random() % TAM;
    } while (posicao_ocupada(e, x, y));
    e.jog.x = x;
    e.jog.y = y;
    do {
        x = random() % TAM;
        y = random() % TAM;
    } while (posicao_ocupada(e, x, y));
    e.saida.x = x;
    e.saida.y = y;
    e = inicializar_inimigos(e, 20);
    e = inicializar_obstaculos(e, 20);
    return e;
}

void imprime_movimento(ESTADO e, int dx, int dy)
{
    ESTADO novo = e;
    int x = e.jog.x + dx;
    int y = e.jog.y + dy;
    char link[MAX_BUFFER];

    if (!posicao_valida(x, y)) return;
    if (posicao_ocupada(e, x, y)) return;
    if (tem_saida(e, x, y)) {
        sprintf(link, "http://localhost/cgi-bin/exemplo");
        ABRIR_LINK(link);
        imprime_casa(e, x, y);
        FECHAR_LINK;
        return;
    }
    novo.jog.x = x;
    novo.jog.y = y;
    sprintf(link, "http://localhost/cgi-bin/exemplo?%s", estado2str(novo));
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

ESTADO ler_estado(char *args)
{
    if(strlen(args) == 0)
        return inicializar();
    return str2estado(args);
}

void imprime_inimigos(ESTADO e)
{
    int i;
    for(i = 0; i < e.num_inimigos; i++)
        IMAGEM(e.inimigo[i].x, e.inimigo[i].y, ESCALA, "Driders_04.png");
}

void imprime_obstaculos(ESTADO e)
{
    int i;
    for(i = 0; i < e.num_obstaculos; i++)
        IMAGEM(e.obstaculo[i].x, e.obstaculo[i].y, ESCALA, "lava_pool1.png");
}

int main()
{
    srandom(time(NULL));
    int x, y;
    ESTADO e = ler_estado(getenv("QUERY_STRING"));
    COMECAR_HTML;
    ABRIR_SVG(600, 600);
    for(y = 0; y < 10; y++) {
        for(x = 0; x < 10; x++) {
            imprime_casa(e, x, y);
            }
        }
    imprime_jogador(e);
    imprime_inimigos(e);
    imprime_obstaculos(e);
    FECHAR_SVG;
    return 0;
}
