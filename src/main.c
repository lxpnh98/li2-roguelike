#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

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

int main()
{
    FILE *file = fopen("/var/www/estado", "r");
    ESTADO e;
    char query[MAX_BUFFER];
    sprintf(query, "%s", getenv("QUERY_STRING"));
    srand(time(NULL));
    COMECAR_HTML;
    e = ler_estado(file, query);
    e = atualizar_estado(e, query);
    if (file != NULL) {
        file = fopen("/var/www/estado", "w");
        fputs(estado2str(e), file);
    }
    
    if (e.jog.vidas > 0) {
        ABRIR_SVG(600, 600);
        imprime_jogo(e);
        imprime_retorno();
        FECHAR_SVG;
    } else {
        ABRIR_SVG(600, 600);
        guardar_pontuacao("/var/www/scores", e.jog);
        imprime_top("/var/www/scores");
        imprime_retorno();
        FECHAR_SVG;
    }
    
    if (file)
        fclose(file);
    return 0;
}
