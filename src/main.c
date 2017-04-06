#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

int main()
{
    /* srandom(time(NULL)); */
    int x, y;
    ESTADO e;
    srand(time(NULL));
    e = ler_estado(getenv("QUERY_STRING"));
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
