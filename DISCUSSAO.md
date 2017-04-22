Bugs:

- Não dá para matar inimigos em certas posições por causa dos obstáculos e assim;
- Testar se a casa de saída pode ser alcançada pelo jogador;

Ideias para features:

- Diferentes tipos de inimigos: Guerreiro, Arqueiro, Cavaleiro e Corredor;
- Criar ficheiro/html "HElP";
- (Forma de mover inimigos).

TODO:

- Mover inimigos;
- "Next level".

Discussão geral (indentar para responder):

Existem muitas funções dentro do ficheiro estado.c, e algumas deles são
utilizadas por html.c. Talvez podiamos criar um ficheiro (aux.c?) para ficar
com estas funções.

As funções são estas: estado2str, str2estado, posicao_valida, posicao_igual,
tem_jogador, tem_inimigo, tem_saida, posicao_ocupada, adjacente, get_z, e
colinear.

-A
