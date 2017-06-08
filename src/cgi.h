#ifndef ___CGI_H___
#define ___CGI_H___

/**
@file cgi.h
Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH                            "http://localhost/images/"

/**
\brief Macro para começar o html
*/
#define COMECAR_HTML                          printf("Content-Type: text/html\n\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)                 printf("<svg width=%d height=%d>\n", tamx, tamy)
/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG                            printf("</svg>\n")

/**
\brief Macro para criar uma imagem
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala da imagem
@param FICHEIRO O caminho para o link do ficheiro
*/
#define IMAGEM(X, Y, X_OFFSET, Y_OFFSET, ESCALA, FICHEIRO) \
    printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
        ESCALA * X + X_OFFSET, ESCALA * Y + Y_OFFSET, ESCALA, ESCALA, IMAGE_PATH FICHEIRO)

/**
\brief Macro para criar um quadrado
@param X A coordenada X do canto superior esquerdo
@param Y A coordenada Y do canto superior esquerdo
@param ESCALA A escala do quadrado
@param COR A cor de preenchimento do quadrado
*/
#define QUADRADO(X, Y, ESCALA, COR) \
    printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n", \
        ESCALA * X, ESCALA* Y, ESCALA, ESCALA, COR)

/**
\brief Macro para abrir um link
@param link O caminho para o link
*/
#define ABRIR_LINK(link)                      printf("<a xlink:href=%s>\n", link)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK                           printf("</a>\n")

/**
\brief Macro para abrir a tabela das pontuções
*/
#define ABRIR_TABELA(X, Y)                    printf("<TABLE border=\"3\" style=\"position:absolute; left:%dpx; top: %dpx;\">\n", X, Y)

/**
\brief Macro para fechar a tabela das pontuções
*/
#define FECHAR_TABELA                         printf("</TABLE>\n")

/**
\brief Macro para abrir uma linha da tabela
*/
#define ABRIR_LINHA                           printf("<TR>\n")

/**
\brief Macro para fechar uma linha da tabela
*/
#define FECHAR_LINHA                          printf("</TR>\n")

/**
\brief Macro para imprimir uma célula da tabela
*/
#define IMPRIMIR_CELULA(X)                    printf("<TD align=center WIDTH=100 >%d</TD>\n", X)

/**
\brief Macro para imprimir o cabeçalhado da tabela, que contém uma string.
*/
#define IMPRIMIR_CABECALHO(S)                 printf("<Th align=center WIDTH=100 >%s</Th>\n", S)

#endif
