#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

int main()
{
    FILE *file = fopen("/var/www/estado", "r+");
    ESTADO e;

    srand(time(NULL));
    e = ler_estado(file, getenv("QUERY_STRING"));
    if (file != NULL) {
        fputs(estado2str(e), file);
    }

    COMECAR_HTML;
    ABRIR_SVG(600, 600);
    imprime_tabuleiro(e);
    imprime_jogador(e);
    imprime_inimigos(e);
    imprime_obstaculos(e);
    FECHAR_SVG;
    if (file)
        fclose(file);
    return 0;
}
