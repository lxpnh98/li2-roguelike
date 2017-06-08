#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cgi.h"
#include "estado.h"
#include "html.h"

int main()
{
    FILE *fp = fopen("/var/www/estado", "r");
    ESTADO e;
    char query[MAX_BUFFER];
    sprintf(query, "%s", getenv("QUERY_STRING"));
    srand(time(NULL));
    e = ler_estado(fp, query);
    e = atualizar_estado(e, query);
    guardar_estado(fp, e);
    imprime_estado(e);
    if (fp)
        fclose(fp);
    return 0;
}
