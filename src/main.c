#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

void guardar_pontuacao(char top_scores[], int score)
{
    int i, j;
    int scores[10];
    
    FILE *file = fopen(top_scores, "r");
    
    /* Carregar pontuações mais altas, preenchendo o resto da array com 0s. */
    for (i = 0; fscanf(file, "%d\n", &scores[i]) == 1 && i < 10; i++);
    for (; i < 10; i++) scores[i] = 0;
    fclose(file);

    /* Inserir pontuação na array ordenada. */
    for (i = 0; i < 10; i++) {
        if (score > scores[i]) {
            for (j = 9; j > i; j--)
                scores[j] = scores[j-1];
            scores[j] = score;
            break;
        }
    }

    /* Escrever o top das pontuações atualizado para o ficheiro. */
    file = fopen(top_scores, "w+");
    for (i = 0; i < 10; i++) {
        fprintf(file, "%d\n", &scores[i]);
    }
}

int main()
{
    FILE *file = fopen("/var/www/estado", "r");
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
        guardar_pontuacao("/var/www/scores", e.score);
        imprime_top();
    }
    FECHAR_SVG;
    if (file)
        fclose(file);
    return 0;
}
