#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

void guardar_pontuacao(int score)
{}

int main()
{
    FILE *file = fopen("/var/www/estado", "r");
    FILE *top_scores = fopen("/var/www/scores", "r");
    ESTADO e;
    char query[MAX_BUFFER];
    sprintf(query, "%s", getenv("QUERY_STRING"));
    srand(time(NULL));
    e = ler_estado(file, query);
    e = atualizar_estado(e, query);
    if (file != NULL) {
        file = fopen("/var/www/estado", "w");
        fputs(estado2str(e), file);
    }

    COMECAR_HTML;
    ABRIR_SVG(600, 600);
    if (e.vidas > 0) {
        imprime_jogo(e);
    } else {
        top_scores = fopen("/var/www/scores", "w");
        guardar_pontuacao(e.score);
        imprime_top();
    }
    FECHAR_SVG;
    if (file)
        fclose(file);
    return 0;
}
