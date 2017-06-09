/**
@file calc.c
Funções auxiliares à inicialização e atualização do estado.
*/

#include <math.h>
#include <stdlib.h>

#include "estado.h"
#include "calc.h"
/**
    \brief Função que calcula a coordenada z de uma dada estrutura que representa uma posição.
           Esta função recebe uma variavel p, do tipo POSICAO, e retorna a componente z dessa mesma posição.

    @param p Estrutura que representa uma posição.
    @retuns coordenada z do ponto.
 */
int get_z(POSICAO p)
{
    return -(p.x + p.y);
}

/**
    \brief Função que verifica se uma posição é válida.
           Esta função recebe uma variavel p, do tipo POSICAO, e retorna uma variavel do tipo boleano 
           como resultado do teste que deverá ser verdadeira caso a posição seja válida.

    @param p estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int posicao_valida(POSICAO p)
{
    POSICAO center_pos;
    center_pos.x = TAM / 2;
    center_pos.y = TAM / 2;
    return abs(p.x - TAM / 2)                <= TAM / 2 &&
           abs(p.y - TAM / 2)                <= TAM / 2 &&
           abs(get_z(p) - get_z(center_pos)) <= TAM / 2;
}

/**
    \brief Função que verifica se um movimento é válido.
           Esta função recebe duas variaveis inteiras e retorna uma variavel do tipo boleano como 
           resultado do teste que deverá ser verdadeira no caso de o movimento ser válido.


    @param dx uma variavel inteira.
    @param dy uma variavel inteira.
    @retuns O resultado do teste.
*/
int movimento_valido(int dx, int dy)
{
    POSICAO p;
    p.x = dx; 
    p.y = dy; 
    return abs(dx)       < 2 &&
           abs(dy)       < 2 &&
           abs(get_z(p)) < 2;
}

/**
    \brief Função que verifica se um movimento é válido para o inimigo do tipo cavaleiro.
           Esta função recebe duas variaveis inteiras e retorna uma variavel do tipo boleano como 
           resultado do teste que deverá ser verdadeira caso o movimento seja válido para o um
           inimigo do tipo cavaleiro.

    @param dx uma variavel inteira.
    @param dy uma variavel inteira.
    @retuns O resultado do teste.
*/
int movimento_valido_cav(int dx, int dy)
{
    return (dx == +2 && dy ==  0) ||
           (dx == -2 && dy ==  0) ||
           (dx ==  0 && dy == -2) ||
           (dx ==  0 && dy == +2) ||
           (dx == +2 && dy == -2) ||
           (dx == -2 && dy == +2) ||
           (dx == +1 && dy == -2) ||
           (dx == -1 && dy == +2) ||
           (dx == +2 && dy == -1) ||
           (dx == -2 && dy == +1) ||
           (dx == +1 && dy == +1) ||
           (dx == -1 && dy == -1);
}

/**
    \brief Função que gera posições aleatórias.
           Esta função recebe uma variavel e, do tipo ESTADO, um apontador para uma 
           variavel p, do tipo POSICAO, e uma variavel do tipo boleano e gera posições aleatórias a partir delas.

    @param e estrutura que representa o estado.
    @param p estrutura que representa uma posição.
    @param testar_saida uma variável do tipo boleano.
*/
void rand_pos(ESTADO e, POSICAO *p, int testar_saida)
{
    do {
        p->x = rand() % TAM;
        p->y = rand() % TAM;
    } while (!posicao_valida(*p)    ||
             posicao_ocupada(e, *p) ||
             (tem_saida(e, *p) && testar_saida));
}

/**
    \brief Função que verifica se duas posições são iguais.
           Esta função recebe duas variaveis p1 e p2, do tipo POSICAO, e retorna uma variavel 
           do tipo boleano como resultado do teste que deverá ser verdadeira no caso de as
           posições serem iguais. 

    @param p1 estrutura que representa uma posição.
    @param p2 estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int posicao_igual(POSICAO p1, POSICAO p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

/**
    \brief Função que verifica se uma dada posição está ocupada pelo jogador.
           Esta função recebe uma variavel e, do tipo ESTADO, e uma variavel p, do tipo POSICAO, e retorna 
           uma variavel do tipo boleano como resultado do teste que deverá ser verdadeira caso essa
           posição seja o jogador.

    @param e estrutura que representa o estado.
    @param p estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int tem_jogador(ESTADO e, POSICAO p)
{
    return posicao_igual(e.jog.pos, p);
}

/**
    \brief Função que verifica se uma dada posição está ocupada por um inimigo.
           Esta função recebe uma variavel e, do tipo ESTADO, e uma variavel p, do tipo POSICAO, e retorna 
           uma variavel do tipo boleano como resultado do teste que deverá ser verdadeira no caso de a
           posição ser um inimigo.

    @param e estrutura que representa o estado.
    @param p estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int tem_inimigo(ESTADO e, POSICAO p)
{
    int i, r = 0;
    for (i = 0; i < e.num_inimigos; i++) {
        if (posicao_igual(e.inimigo[i].pos, p)) {
            r = 1;
            break;
        }
    }
    return r;
}

/**
    \brief Função que verifica se uma dada posição está ocupada por um obstáculo.
           Esta função recebe uma variavel e, do tipo ESTADO, e uma variavel p, do tipo POSICAO, e retorna 
           uma variavel do tipo boleano como resultado do teste que deverá ser verdadeira caso essa
           posição seja um obstaculo.

    @param e estrutura que representa o estado.
    @param p estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int tem_obstaculo(ESTADO e, POSICAO p)
{
    int i, r = 0;
    for (i = 0; i < e.num_obstaculos; i++) {
        if (posicao_igual(e.obstaculo[i], p)) {
            r = 1;
            break;
        }
    }
    return r;
}

/**
    \brief Função que verifica se uma dada posição é a saida.
           Esta função recebe uma variavel e, do tipo ESTADO, e uma variavel p, do tipo POSICAO, e retorna 
           uma variavel do tipo boleano como resultado do teste que deverá ser verdadeira no caso de a
           posição ser a saída.

    @param e estrutura que representa o estado.
    @param p estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int tem_saida(ESTADO e, POSICAO p)
{
    return posicao_igual(e.saida, p);
}

/**
    \brief Função que verifica se uma dada posição está ocupada.
           Esta função recebe uma variavel e, do tipo ESTADO, e uma variavel p, do tipo POSICAO, e retorna 
           uma variavel do tipo boleano como resultado do teste que deverá ser verdadeira no caso de a posição
           esteja ocupada.

    @param e estrutura que representa o estado.
    @param p estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int posicao_ocupada(ESTADO e, POSICAO p)
{
    return tem_jogador(e, p) ||
           tem_inimigo(e, p) ||
           tem_obstaculo(e, p);
}

/**
    \brief Função que verifica se duas posições são adjacentes.
           Esta função recebe duas variaveis p1 e p2, do tipo POSICAO, e retorna uma variavel 
           do tipo boleano como resultado do teste que deverá ser verdadeira no caso de as 
           posições serem adjacentes. 

    @param p1 estrutura que representa uma posição.
    @param p2 estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int adjacente(POSICAO p1, POSICAO p2)
{
    return (abs(p1.x - p2.x)           < 2) &&
           (abs(p1.y - p2.y)           < 2) &&
           (abs(get_z(p1) - get_z(p2)) < 2);
}

/**
    \brief Função que verifica se três posições são colineares.
           Esta função recebe três variaveis p1, p2 e p3, do tipo POSICAO, e retorna uma variavel 
           do tipo boleano como resultado do teste que deverá ser verdadeira no caso de as 
           posições serem olineares. 

    @param p1 estrutura que representa uma posição.
    @param p2 estrutura que representa uma posição.
    @param p3 estrutura que representa uma posição.
    @retuns O resultado do teste.
*/
int colinear(POSICAO p1, POSICAO p2, POSICAO p3)
{
    return (p1.x == p2.x && p2.x == p3.x) ||
           (p1.y == p2.y && p2.y == p3.y) ||
           (get_z(p1) == get_z(p2) && get_z(p2) == get_z(p3));
}

