#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "estado.h"
#include "calc.h"

#define INIT_VIDAS              5

char *estado2str(ESTADO e)
{
    static char buffer[MAX_BUFFER];
    char *p = (char *) &e;
    int i;

    buffer[0] = 0;

    for(i = 0; (unsigned int)i < sizeof(ESTADO); i++)
        sprintf(buffer, "%s%02x", buffer, p[i]);
    
    return buffer;
}

ESTADO str2estado(char *argumentos)
{
    ESTADO e;
    char *p = (char *) &e;
    int i;

    for(i = 0; (unsigned int)i < sizeof(ESTADO); i++, argumentos += 2) {
        int d;
        sscanf(argumentos, "%2x", (unsigned int *)&d);
        p[i] = (char) d;
    }
    
    return e;
}

ESTADO inicializar_inimigo(ESTADO e) {
    POSICAO p;
    rand_pos(e, &p, 1);
    e.inimigo[(int)e.num_inimigos].tipo = rand() % 3;
    e.inimigo[(int)e.num_inimigos].pos = p;
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
    POSICAO p;
    rand_pos(e, &p, 1);
    e.obstaculo[(int)e.num_obstaculos] = p;
    e.num_obstaculos++;

    return e;
}

ESTADO inicializar_obstaculos(ESTADO e, int num) {
    int i;
    for (i = 0; i < num; i++)
        e = inicializar_obstaculo(e);
    return e;
}

ESTADO inicializar(int vidas, int score) {
    POSICAO p;
    ESTADO e = {{{0,0},0,0,0},0,0,{{0}},{{0}},{0,0}};
    rand_pos(e, &p, 0);
    e.jog.pos = p;
    e.jog.vidas = vidas;
    e.jog.score = score;
    e.jog.modo = NORMAL;
    rand_pos(e, &p, 0);
    e.saida = p;
    e = inicializar_inimigos(e, 10);
    e = inicializar_obstaculos(e, 10);
    return e;
}

ESTADO ler_estado(FILE *file, char query[])
{
    char *e;
    int tamanho;
    int pagina, vidas, score;
    char modo;

    /* Se não existir ficheiro , criar estado aleatório. */
    if (file == NULL)
        return inicializar(INIT_VIDAS, 0);

    /* Se estiver no menu principal ou jogador passar de nível, gerar estado
     * aleatório */
    sscanf(query, "%d", &pagina);
    if (pagina == 1) {
        sscanf(query, "1,%c", &modo);
        if (modo == 'x') {
            sscanf(query, "1,x,%d,%d", &vidas, &score);
            return inicializar(vidas, score);
        }
    }

    /* Descubrir tamanho do ficheiro. */
    fseek(file, 0, SEEK_END);
    tamanho = ftell(file);
    rewind(file);

    /* Se ficheiro existir mas for vazio, criar estado aleatório. */
    if (tamanho == 0)
        return inicializar(INIT_VIDAS, 0);

    /* Alocar memória necessária, copiar conteúdos do ficheiro para a variável
     * e terminar o string. */
    e = (char *)malloc(sizeof(char) * (tamanho + 1));
    tamanho = fread(e, sizeof(char), tamanho, file);
    e[tamanho] = '\0';

    /* Converter o string para estado e retornar. */
    return str2estado(e);
}

void preencher_guerreiro(int m[TAM][TAM], ESTADO e, int x, int y, int dist)
{
    POSICAO p;
    p.x = x;
    p.y = y;
    if (m[y][x] > dist && posicao_valida(p) && 
        !tem_obstaculo(e, p) && !tem_inimigo(e, p)) {
        m[y][x] = dist;
        preencher_guerreiro(m, e, x + 1, y    , dist + 1);
        preencher_guerreiro(m, e, x - 1, y    , dist + 1);
        preencher_guerreiro(m, e, x    , y - 1, dist + 1);
        preencher_guerreiro(m, e, x    , y + 1, dist + 1);
        preencher_guerreiro(m, e, x + 1, y - 1, dist + 1);
        preencher_guerreiro(m, e, x - 1, y + 1, dist + 1);
    }
}

void preencher_cavaleiro(int m[TAM][TAM], ESTADO e, int x, int y, int dist)
{
    POSICAO p;
    p.x = x;
    p.y = y;
    if (m[y][x] > dist && posicao_valida(p) && 
        !tem_obstaculo(e, p) && !tem_inimigo(e, p)) {
        m[y][x] = dist;
        preencher_cavaleiro(m, e, x + 2, y    , dist + 1);
        preencher_cavaleiro(m, e, x - 2, y    , dist + 1);
        preencher_cavaleiro(m, e, x    , y - 2, dist + 1);
        preencher_cavaleiro(m, e, x    , y + 2, dist + 1);
        preencher_cavaleiro(m, e, x + 2, y - 2, dist + 1);
        preencher_cavaleiro(m, e, x - 2, y + 2, dist + 1);
        preencher_cavaleiro(m, e, x + 1, y - 2, dist + 1);
        preencher_cavaleiro(m, e, x - 1, y + 2, dist + 1);
        preencher_cavaleiro(m, e, x + 2, y - 1, dist + 1);
        preencher_cavaleiro(m, e, x - 2, y + 1, dist + 1);
        preencher_cavaleiro(m, e, x + 1, y + 1, dist + 1);
        preencher_cavaleiro(m, e, x - 1, y - 1, dist + 1);
    }
}

void matriz_guerreiro(ESTADO e, int m[TAM][TAM])
{
    int i, j;
    for (i = 0; i < TAM; i++)
        for (j = 0; j < TAM; j++)
            m[i][j] = 999;
    preencher_guerreiro(m, e, e.jog.pos.x, e.jog.pos.y, 0);
}

void matriz_cavaleiro(ESTADO e, int m[TAM][TAM])
{
    int i, j;
    for (i = 0; i < TAM; i++)
        for (j = 0; j < TAM; j++)
            m[i][j] = 999;
    preencher_cavaleiro(m, e, e.jog.pos.x, e.jog.pos.y, 0);
}

void mover_cavaleiro(ESTADO *e, int n, int m_cavaleiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    if (!adjacente(inimigo->pos, e->jog.pos)) {
        for (i = inimigo->pos.x - 2; i <= inimigo->pos.x + 2; i++)
            for (j = inimigo->pos.y - 2; j <= inimigo->pos.y + 2; j++) {
                p.x = i;
                p.y = j;
                if (posicao_valida(p) &&
                    !posicao_ocupada(*e, p) &&
                    movimento_valido_cav(inimigo->pos.x - i, inimigo->pos.y - j) &&
                    m_cavaleiro[j][i] <= m_cavaleiro[ny][nx]) {
                    nx = i;
                    ny = j;                    
                }
            }
    }
    inimigo->pos.x = nx;
    inimigo->pos.y = ny;
}

void mover_guerreiro(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    if (!adjacente(inimigo->pos, e->jog.pos)) {
        for (i = inimigo->pos.x - 1; i <= inimigo->pos.x + 1; i++)
            for (j = inimigo->pos.y - 1; j <= inimigo->pos.y + 1; j++) {
                p.x = i;
                p.y = j;
                if (posicao_valida(p) &&
                    !posicao_ocupada(*e, p) &&
                    movimento_valido(inimigo->pos.x - i, inimigo->pos.y - j) &&
                    m_guerreiro[j][i] < m_guerreiro[ny][nx]) {
                    nx = i;
                    ny = j;                    
                }
            }
    }
    inimigo->pos.x = nx;
    inimigo->pos.y = ny;
}

void mover_corredor(ESTADO *e, int n, int m_guerreiro[TAM][TAM])
{
    int i, j;
    INIMIGO *inimigo = &(e->inimigo[n]);
    int nx = inimigo->pos.x;
    int ny = inimigo->pos.y;
    POSICAO p;
    for (i = inimigo->pos.x - 1; i <= inimigo->pos.x + 1; i++)
        for (j = inimigo->pos.y - 1; j <= inimigo->pos.y + 1; j++) {
            p.x = i;
            p.y = j;
            if (posicao_valida(p) &&
                !posicao_ocupada(*e, p) &&
                movimento_valido(inimigo->pos.x - i, inimigo->pos.y - j) &&
                m_guerreiro[j][i] < m_guerreiro[ny][nx]) {
                nx = i;
                ny = j;                    
            }
        }
    inimigo->pos.x = nx;
    inimigo->pos.y = ny;
}

void mover_inimigo(ESTADO *e, int n, int m_guerreiro[TAM][TAM], int m_cavaleiro[TAM][TAM])
{
    INIMIGO *inimigo = &(e->inimigo[n]);
    switch (inimigo->tipo) {
        case GUERREIRO:
            mover_guerreiro(e, n, m_guerreiro);
            break;
        case CORREDOR:
            mover_corredor(e, n, m_guerreiro);
            break;
        case CAVALEIRO:
            mover_cavaleiro(e, n, m_guerreiro /* m_cavaleiro */);
            break;
    }
}

void matar_jogador(ESTADO *e, ESTADO antigo)
{
    int i;
    for (i = 0; i < e->num_inimigos; i++) {
        switch (e->inimigo[i].tipo) {
            case GUERREIRO:
                if (posicao_igual(e->inimigo[i].pos, antigo.inimigo[i].pos) &&
                    adjacente(e->jog.pos, e->inimigo[i].pos))
                    e->jog.vidas--;
                break;
            case CORREDOR:
                if (colinear(e->inimigo[i].pos, antigo.inimigo[i].pos, e->jog.pos) &&
                    adjacente(e->jog.pos, e->inimigo[i].pos))
                    e->jog.vidas--;
                break;
            case CAVALEIRO:
                if (!posicao_igual(e->inimigo[i].pos, antigo.inimigo[i].pos) &&
                    adjacente(e->jog.pos, antigo.inimigo[i].pos) &&
                    adjacente(e->jog.pos, e->inimigo[i].pos))
                    e->jog.vidas--;
                break;
        }
    }
    if (e->jog.vidas < 0)
        e->jog.vidas = 0;
}

void eliminar_inimigo(ESTADO *e, int n)
{
    int i;
    for (i = n + 1; i <= e->num_inimigos; i++)
        e->inimigo[i - 1] = e->inimigo[i];
    e->num_inimigos--;
}

POSICAO nova_posicao(POSICAO antiga, char mov)
{
    switch (mov) {
        case 'r':
            antiga.x++;
            break;
        case 'l':
            antiga.x--;
            break;
        case 'u':
            antiga.y--;
            break;
        case 'd':
            antiga.y++;
            break;
        case 'f':
            antiga.x++;
            antiga.y--;
            break;
        case 'b':
            antiga.x--;
            antiga.y++;
            break;
    }
    return antiga;
}

void atualizar_normal(ESTADO *e, ESTADO *antigo, char mov)
{
    POSICAO nova_pos;
    int i;
    int adj, lunge;
    nova_pos = nova_posicao(e->jog.pos, mov);
    for (i = 0; i < e->num_inimigos; i++) {
        adj = adjacente(e->inimigo[i].pos, e->jog.pos);
        lunge = colinear(e->inimigo[i].pos, e->jog.pos, nova_pos);
        if (adjacente(e->inimigo[i].pos, nova_pos) && (adj || lunge)) {
            eliminar_inimigo(e, i);
            eliminar_inimigo(antigo, i);
            i--;
            e->jog.score++;
        }
    }
    e->jog.pos = nova_pos;
}

void atualizar_ataque(ESTADO *e, ESTADO *antigo, char mov)
{
    int i;
    POSICAO pos_ataque = nova_posicao(e->jog.pos, mov);
    for(i = 0; i < e->num_inimigos; i++){
        if (posicao_igual(pos_ataque, e->inimigo[i].pos)) {
            eliminar_inimigo(e, i);
            eliminar_inimigo(antigo, i);
            e->jog.score++;
            break;
        }
    }
    /* ver se é igual
    // loop dos inimigos
       eliminar inimigo i */

    e->jog.modo = 'n';
}

void atualizar_modo(ESTADO *e, char modo)
{
    e->jog.modo = modo;
}

ESTADO atualizar_estado(ESTADO e, char query[])
{
    ESTADO antigo = e;
    int pagina;
    char modo, mov;
    int i;
    int m_guerreiro[TAM][TAM];
    int m_cavaleiro[TAM][TAM];

    sscanf(query, "%d", &pagina);
    //%c,%c", (char *)&e.jog.modo, &mov);

    if (pagina == 1) {
        sscanf(query, "1,%c", &modo);
        if (e.jog.vidas <= 0 || modo == 'x')
            return e;
        sscanf(query, "1,%c,%c", (char *)&e.jog.modo, &mov);
        
        switch (e.jog.modo) {
            case NORMAL:
                atualizar_normal(&e, &antigo, mov);
                break;
            case ATAQUE:
                atualizar_ataque(&e, &antigo, mov);
                break;
            default:
                break;
        }

        /* Mover inimigos aqui */
        if (e.jog.modo != MUDAR_MODO) {
            matriz_guerreiro(e, m_guerreiro);
            for (i = 0; i < e.num_inimigos; i++) {
                mover_inimigo(&e, i, m_guerreiro, m_cavaleiro);
            }

            matar_jogador(&e, antigo);
        } else {
            atualizar_modo(&e, mov);
        }
        return e;

    } else {
        return inicializar(INIT_VIDAS, 0);
    }

    /*
    if (e.jog.vidas <= 0 || query[0] == 'x')
        return e;
    sscanf(query, "%c,%c", (char *)&e.jog.modo, &mov);
    printf("<!-- %c -->\n", e.jog.modo);

    switch (e.jog.modo) {
        case NORMAL:
            atualizar_normal(&e, &antigo, mov);
            break;
        case ATAQUE:
            atualizar_ataque(&e, &antigo, mov);
            break;
        default:
            break;
    }

    if (e.jog.modo != MUDAR_MODO) {
        matriz_guerreiro(e, m_guerreiro);
        for (i = 0; i < e.num_inimigos; i++) {
            mover_inimigo(&e, i, m_guerreiro, m_cavaleiro);
        }

        matar_jogador(&e, antigo);
    } else {
        atualizar_modo(&e, mov);
    }
    */

}

