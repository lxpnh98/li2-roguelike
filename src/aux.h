int posicao_valida(int x, int y);

int movimento_valido(int dx, int dy);

void rand_pos(ESTADO e, int *x, int *y, int testar_saida);

int posicao_igual(POSICAO p, int x, int y);

int tem_jogador(ESTADO e, int x, int y);

int tem_inimigo(ESTADO e, int x, int y);

int tem_obstaculo(ESTADO e, int x, int y);

int tem_saida(ESTADO e, int x, int y);

int posicao_ocupada(ESTADO e, int x, int y);

int get_z(POSICAO p);

int adjacente(POSICAO p1, POSICAO p2);

int colinear(POSICAO p1, POSICAO p2, POSICAO p3);

