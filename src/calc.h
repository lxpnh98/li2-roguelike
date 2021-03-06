/**
@file calc.h
Definições das funções auxiliares à inicialização e atualização do estado.
*/

int movimento_valido(int dx, int dy);

int movimento_valido_cav(int dx, int dy);

void rand_pos(ESTADO e, POSICAO *p, int testar_saida);

int posicao_igual(POSICAO p1, POSICAO p2);

int posicao_valida(POSICAO p);

int tem_inimigo(ESTADO e, POSICAO p);

int tem_obstaculo(ESTADO e, POSICAO p);

int tem_saida(ESTADO e, POSICAO p);

int posicao_ocupada(ESTADO e, POSICAO p);

int adjacente(POSICAO p1, POSICAO p2);

int colinear(POSICAO p1, POSICAO p2, POSICAO p3);

