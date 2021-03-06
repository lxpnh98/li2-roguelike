/**
@file ficheiro.c
Funções de conversão entre string e ESTADO, de leitura e escrita do estado e das pontuações.
*/

#include <stdio.h>
#include <stdlib.h>

#include "ficheiro.h"

/**
\brief Função que converte o estado em uma string.
       Esta função recebe uma variavel e, do tipo ESTADO, e retorna uma variavel 
       do tipo char, isto é, o ESTADO convertido para string. 
@param e estrutura que representa o estado.
@returns variavel do tipo char que representa o estado.
*/
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

/**
\brief Função que converte uma string em um estado.
       Esta função recebe um apontador para uma variavel do tipo char e retorna uma variavel 
       do tipo ESTADO, isto é, a string é convertida em um ESTADO.
@param argumentos um apontador para uma string.
@returns Uma estrutura que representa o estado.
*/
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

/**
\brief Função que lê o estado.
       Esta função recebe um apontador para um fichero e uma string e retorna uma variavel 
       do tipo ESTADO.
@param file uma variavel do tipo FILE.
@param query uma string, do tipo char.
@returns Uma estrutura que representa o estado.
*/
ESTADO ler_estado(FILE *file, char query[])
{
    char *e;
    int tamanho;
    int pagina, vidas, score;
    char modo;
    // Se não existir ficheiro, criar estado aleatório.
    if (file == NULL)
        return inicializar(INIT_VIDAS, 0);
    // Se estiver no menu principal ou jogador passar de nível, gerar estado
    // aleatório.
    sscanf(query, "%d", &pagina);
    if (pagina == 1) {
        sscanf(query, "1,%c", &modo);
        if (modo == 'x') {
            sscanf(query, "1,x,%d,%d", &vidas, &score);
            return inicializar(vidas, score);
        }
    }
    // Descubrir tamanho do ficheiro.
    fseek(file, 0, SEEK_END);
    tamanho = ftell(file);
    rewind(file);
    // Se ficheiro existir mas for vazio, criar estado aleatório.
    if (tamanho == 0)
        return inicializar(INIT_VIDAS, 0);
    // Alocar memória necessária, copiar conteúdos do ficheiro para a variável
    // e terminar o string.
    e = (char *)malloc(sizeof(char) * (tamanho + 1));
    tamanho = fread(e, sizeof(char), tamanho, file);
    e[tamanho] = '\0';
    // Converter o string para estado e retornar.
    return str2estado(e);
}

/**
\brief Função que guarda o estado.
       Esta função recebe um apontador para um fichero e uma variável e do tipo ESTADO e guarda o estado.
@param fp uma variavel do tipo FILE.
@param e estrutura que representa o estado.
*/
void guardar_estado(FILE *fp, ESTADO e)
{
    if (fp != NULL) {
        fp = fopen("/var/www/estado", "w");
        fputs(estado2str(e), fp);
    }
}

/**
\brief Função que guarda as pontuações.
       Esta função recebe uma string e uma variável jog do tipo JOGADOR e guarda a pontuação.
@param top_scores uma string, do tipo char.
@param jog estrutura que representa o jogador.
*/
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

