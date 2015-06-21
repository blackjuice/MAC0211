/*************************************************************************
 *  integrantes do grupo:           n.USP:
 *      William Shinji Numada       764 8325 
 *      Lucas Sung Jun Hong         812 4329
 *
 *  Segunda Fase do projeto Bombardeio Naval
 *
 *  execução: ./configure && make && ./ep3_7648325_8124329
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "xwc.h"

#define MAX_NOME                    128     // tamanho maximo para nome dos arquivos
#define SAFE_MOVE                   0
#define INVALID_MOVE                1
#define GAME_OVER_BY_COLLISION      10
#define GAME_OVER_BY_ATTACK         11
#define GAME_OVER_SUCCESS           12
#define UP                          15
#define DOWN                        16
#define LEFT                        17
#define RIGHT                       18

/**************************************
 *      Declaracao das funcoes
 **************************************/
char ** leia_mapa                   (char *nomeArquivo, int *linha, int *coluna);
char ** mallocImagemGame            (int *linha, int *coluna);
void    escreva_mapa_tela           (char **mapa, int linha, int coluna);
void    escreva_mapa_arquivo        (char *nomeArquivo, char **mapa, int linha, int coluna);
void    escreva_mapa_arquivo_situa  (char *nome, int direcao);
void    escreva_mapa_arquivo_tiro   (char *nome, int linhaTiro, int colunaTiro);
void    posiciona_barco             (char **mapa, int *coluna);
int     rema_barco                  (char **mapa, int *linAtual, int *colAtual, int rowMax, int columnMax, int *direcao);
void    freeImagemGame              (char **mapa, int linha);

int     sorteia                     (int k);
void    coordenadas_tiro            (int *linhaTiro, int *colunaTiro ,int linha, int coluna);
int     dispara_tiros               (char **mapa, int linha, int coluna, char *nome);
int     identifica_alvo_atingido    (char **mapa, int linhaTiro, int colunaTiro, int linha, int coluna);

void    afunda_destroyer            (char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador);
void    afunda_hidro_aviao          (char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador);
void    afunda_porta_aviao          (char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador);
void    afunda_cruzador             (char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador);

/**************************************
 *      Inicio da main()
 **************************************/
int main(int ac, char **av)
{
    PIC P1, P2, Aux;
    PIC Background;

    int i, j;
    int x, y;
    int sizeX, sizeY;
 
    #ifdef NOXPM
       puts("Este programa só funciona com a biblioteca Xpm!");
    #else
 
    WINDOW  *w1;
    MASK    msk;

    printf("indicate window size\n");
    scanf("%d %d", &sizeX, &sizeY);

    // inicializando janela
    w1 = InitGraph(sizeX, sizeY, "Battlefield 777");
    //w1 = InitGraph(500, 500, "Battlefield 777");

    // leitura das imagens
    //P1 = ReadPic(w1, "samp.xpm", NULL);
    //P2 = ReadPic(w1, "sampT.xpm", NULL);
    //msk = NewMask(P1, 32, 32);
    //Aux = ReadPic(w1, "mascara.xpm", msk);

    // plano de fundo
    Background = ReadPic(w1, "backgroundBasic.xpm", NULL);
        for ( i = 0; i < sizeX; i++ ) {
            for ( j = 0; j < sizeY; j++ ) {
                PutPic(w1, Background, 0, 0, 100, 100, i, j);        
                j += 89;
            }
            i += 89;
        }
/*
    Background = ReadPic(w1, "backgroundBasic1.xpm", NULL);
    for ( i = 0; i < sizeX; i++ ) {
        for ( j = 0; j < sizeY; j++ ) {
            PutPic(w1, Background, 0, 0, 100, 100, i, j);        
            j += 30;
        }
        i += 30;
    }
*/

/*
    WritePic(P1,"Tutti.xpm", msk);

    printf("indicate samp.xpm position\n");
    scanf("%d %d", &x, &y);
    puts("Desenhando a figura do arquivo samp.xpm.");
    puts("Tecle <enter>"); getchar();
    PutPic(w1, P1, 0, 0, 100, 100, x, y);
    //PutPic(w1, P1, 0, 0, 100, 100, 150, 150);
    //PutPic(w1, P1, 0, 0, 100, 100, 0, 0);
*/

/*
    puts("Desenhando a figura do arquivo sampT.xpm.");
    puts("Tecle <enter>"); getchar();
    PutPic(w1, P2, 150, 150, 100, 100, 0, 0);
*/

/*
    puts("Agora  a figura  do arquivo mascara.xpm.");
    puts("Tecle <enter>"); getchar();
    PutPic(w1, Aux, 0,0, 100, 100, 200, 0);
    //PutPic(w1, Aux, 0,0, 100, 100, 200, 0);

    puts("Sobrepondo a última figura com a primeira\n"
        "e usando sua  própria máscara.");
    SetMask(w1,msk);
    puts("Tecle <enter>"); getchar();
    PutPic(w1, Aux, 0,0, 100, 100, 0, 0);
    puts("Tecle <enter>"); getchar();

    puts("Gravando samp.xpm mascarado em Tutti.xpm.");
    WritePic(P1,"Tutti.xpm", msk);
*/
    puts("Tecle <enter> to finish"); getchar();
    puts("Tecle <enter> to finish"); getchar();

    CloseGraph();
    
    #endif
    return 0;
}


/**************************************
 *      Inicio das funcoes
 **************************************/

/* alocacao da matriz que contera o mapa */
char **
mallocImagemGame(int *linha, int *coluna)
{
    int             i;
    char            **M;
    
    M = malloc ( (*linha) * sizeof(char *) );
    for (i = 0; i < (*linha); i++)
        M[i] = malloc ( (*coluna) * sizeof(char) );

    return M;
}

/* leitura do mapa */
char **
leia_mapa(char *nomeArquivo, int *linha, int *coluna)
{
    FILE           *fEntrada;
    int            i, j;
    char           c;
    char           **mapa;

    fEntrada = fopen(nomeArquivo, "r");
    if (fEntrada == NULL) 
    {
        fprintf(stderr, "ERRO! Nao consegui abrir o arquivo '%s'\n", nomeArquivo);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "\n A mapa '%s' carregado com sucesso.\n\n", nomeArquivo);

    /* inicio da leitura do arquivo */
    fscanf(fEntrada, "%d ", &(*linha));
    fscanf(fEntrada, "%d\n", &(*coluna)) ;
    mapa = mallocImagemGame((&(*linha)), (&(*coluna)) );

    for(i = 0; i < (*linha); i++) {
        for(j = 0; j < (*coluna); j++) {
            fscanf(fEntrada, " %c", &c);
            mapa[i][j] = c;
        }
    }

    fclose(fEntrada);
    return (mapa);
}

/* impressao da tela para o usuario */
void escreva_mapa_tela (char **mapa, int linha, int coluna)
{
    int         i, j;
    printf("\n  ");
    for(j = 0; j < coluna ; j++)
        printf(" %d ", j);
    printf("\n");
    for(i = 0; i < linha; i++) {
        printf("%d ", i);
        for(j = 0; j < coluna ; j++) {
            if (mapa[i][j] != 'T' && mapa[i][j] != 'B' &&
                mapa[i][j] != '=' && mapa[i][j] != '*' &&
                mapa[i][j] != '+' && mapa[i][j] != '!')
                printf(" - ");

            else printf(" %c ", mapa[i][j]);
        }
        printf("\n");
    }
}

/* criacao de arquivo que contera os resultados do mapa */
void escreva_mapa_arquivo (char *nome, char **mapa, int linha, int coluna)
{
    FILE       *fSaida;
    int        i, j;

    /* criaremos um arquivo (w = write) */
    fSaida = fopen(nome, "a+");
    if (fSaida == NULL) 
    {
        fprintf(stderr, "ERRO! arquivo '%s' nao pode ser criado\n", nome);
        exit(EXIT_FAILURE);
    }

    fprintf(fSaida, "\n  ");
    for(j = 0; j < coluna ; j++)
        fprintf(fSaida, "%d ", j);
    fprintf(fSaida, "\n");


    for(i = 0; i < linha; i++) {
        fprintf(fSaida, "%d ", i);
        for(j = 0; j < coluna; j++) {
            fprintf(fSaida, "%c ", mapa[i][j]);
        }
        fprintf(fSaida, "\n");
    }

    fprintf(fSaida, "\n----------------------------------------------------\n");
    fclose(fSaida);
}

void escreva_mapa_arquivo_situa (char *nome, int direcao)
{
    FILE       *fSaida;

    /* criaremos um arquivo (w = write) */
    fSaida = fopen(nome, "a+");
    if (fSaida == NULL) 
    {
        fprintf(stderr, "ERRO! arquivo '%s' nao pode ser criado\n", nome);
        exit(EXIT_FAILURE);
    }

    if (direcao == UP)
        fprintf(fSaida, "\nDirecao >> c\n");
    if (direcao == DOWN)
        fprintf(fSaida, "\nDirecao >> b\n");
    if (direcao == LEFT)
        fprintf(fSaida, "\nDirecao >> e\n");
    if (direcao == RIGHT)
        fprintf(fSaida, "\nDirecao >> d\n");
    if (direcao == INVALID_MOVE)
        fprintf(fSaida, "\nDirecao >> INVALIDA\n");
    if (direcao == GAME_OVER_BY_COLLISION) {
        fprintf(fSaida, "\nVocê perdeu! Colisao foi iminente, pelo visto\n");
        fprintf(fSaida, "\nGAME OVER!\n");
    }
    if (direcao == GAME_OVER_BY_ATTACK) {
        fprintf(fSaida, "\nVocê perdeu! Você foi atingido!\n");
        fprintf(fSaida, "\nGAME OVER!\n");
    }
    if (direcao == GAME_OVER_SUCCESS) {
        fprintf(fSaida, "\nVocê ganhou!\n");
        fprintf(fSaida, "\nGAME OVER!\n");
    }

    fclose(fSaida);
}

void    escreva_mapa_arquivo_tiro   (char *nome, int linhaTiro, int colunaTiro) {
    FILE       *fSaida;

    /* criaremos um arquivo (w = write) */
    fSaida = fopen(nome, "a+");
    if (fSaida == NULL) 
    {
        fprintf(stderr, "ERRO! arquivo '%s' nao pode ser criado\n", nome);
        exit(EXIT_FAILURE);
    }

    fprintf(fSaida, "\nUm tiro atingiu a posição [%d][%d]!", linhaTiro, colunaTiro);

    fclose(fSaida);
}

/* free da matriz */
void
freeImagemGame(char **mapa, int linha)
{
    int    i;
    for (i = 0; i < linha; i++)
        free (mapa[i]);
    free (mapa);
}


void
posiciona_barco (char **mapa, int *coluna)
{

    printf("\nDigite a coluna da primeira linha do mapa em que deseja posicionar o barco: \n");
    scanf("%d",&(*coluna));
    while (mapa[0][(*coluna)] != '.') {
        printf("\nPosicao ocupada. Escolha uma outra posicao: ");
        scanf("%d",&(*coluna));
    }

    mapa[0][(*coluna)] = 'B';
}

/*
retorna:
INVALID_MOVE            - se a posicao for invalida
SAFE_MOVE               - se a posicao for valida
GAME_OVER_BY_COLLISION  - se houve colisao
*/
int 
rema_barco (char **mapa, int *linAtual, int *colAtual, int rowMax, int columnMax, int *direcao) {
    char escolha;

    printf( "  _______________________ \n"
            " |                       |\n"
            " |   Direcao do barco:   |\n"
            " |_______________________|\n"
            " |                       |\n"
            " |    (c) para cima      |\n"
            " |    (b) para baixo     |\n"
            " |    (d) para direita   |\n"
            " |    (e) para esquerda  |\n"
            " |_______________________|\n\n"
            "      Direcao >> ");

    scanf(" %c", &escolha);

    if(escolha == 'c') {
        (*direcao) = UP;
        // estamos no topo do mapa. Subir sera invalido.
        if((*linAtual) == 0)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual) - 1][(*colAtual)] == 'S' || mapa[(*linAtual) - 1][(*colAtual)] == 'D' || 
                mapa[(*linAtual) - 1][(*colAtual)] == 'C' || mapa[(*linAtual) - 1][(*colAtual)] == 'P' ||
                mapa[(*linAtual) - 1][(*colAtual)] == 'H')
            { 
                mapa[(*linAtual)][(*colAtual)]  = 'T';
                (*linAtual)--;
                return GAME_OVER_BY_COLLISION;
            }
        else {
                mapa[(*linAtual)][(*colAtual)]  = 'T';
                mapa[(*linAtual) - 1][(*colAtual)] = 'B';
                (*linAtual)--;
                return SAFE_MOVE;
        }
    }

    if(escolha == 'b') {
        (*direcao) = DOWN;
        // estamos na base do mapa. Descer sera invalido
        if((*linAtual) == rowMax)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual) + 1][(*colAtual)] == 'S' || mapa[(*linAtual) + 1][(*colAtual)] == 'D' || 
                mapa[(*linAtual) + 1][(*colAtual)] == 'C' || mapa[(*linAtual) + 1][(*colAtual)] == 'P' || 
                mapa[(*linAtual) + 1][(*colAtual)] == 'H')
            {
                mapa[(*linAtual)][(*colAtual)]  = 'T';
                (*linAtual)++;
                return GAME_OVER_BY_COLLISION;
            }
        else {
                mapa[(*linAtual)][(*colAtual)]  = 'T';
                mapa[(*linAtual) + 1][(*colAtual)] = 'B';
                (*linAtual)++;
                return SAFE_MOVE;
        }
    }

    if(escolha == 'e') {
        (*direcao) = LEFT;
        // estamos na borda esquerda. Ir para esquerda sera invalido
        if((*colAtual) == 0)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual)][(*colAtual) - 1] == 'S' || mapa[(*linAtual)][(*colAtual) - 1] == 'D' || 
                mapa[(*linAtual)][(*colAtual) - 1] == 'C' || mapa[(*linAtual)][(*colAtual) - 1] == 'P' ||
                mapa[(*linAtual)][(*colAtual) - 1] == 'H')
            {
                mapa[(*linAtual)][(*colAtual)]  = 'T';
                (*colAtual)--;
                return GAME_OVER_BY_COLLISION;
            }
        else {
                mapa[(*linAtual)][(*colAtual)] = 'T';
                mapa[(*linAtual)][(*colAtual) - 1] = 'B';
                (*colAtual)--;
                return SAFE_MOVE;
        }
    }

    if(escolha == 'd') {
        (*direcao) = RIGHT;
        // estamos na borda direita. Ir para direita sera invalido
        if((*colAtual) == columnMax)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual)][(*colAtual) + 1] == 'S' || mapa[(*linAtual)][(*colAtual) + 1] == 'D' ||
                mapa[(*linAtual)][(*colAtual) + 1] == 'C' || mapa[(*linAtual)][(*colAtual) + 1] == 'P' ||
                mapa[(*linAtual)][(*colAtual) + 1] == 'H')
            {
                mapa[(*linAtual)][(*colAtual)]  = 'T';
                (*colAtual)++;
                return GAME_OVER_BY_COLLISION;
            }
        else {
                mapa[(*linAtual)][(*colAtual)] = 'T';
                mapa[(*linAtual)][(*colAtual) + 1] = 'B';
                (*colAtual)++;
                return SAFE_MOVE;
        }
    }
    return INVALID_MOVE;
}

int sorteia (int k) {
    return (int)(1 + (rand() / (RAND_MAX + 1.0)) * k);
}

void coordenadas_tiro (int *linhaTiro, int *colunaTiro ,int linha, int coluna) {
    *linhaTiro  = sorteia(linha - 1);
    *colunaTiro = sorteia(coluna - 1);
}

int dispara_tiros (char **mapa, int linha, int coluna, char *nome)
{
    int    tiros;
    int    situation, situation_critical = SAFE_MOVE;
    int    linhaTiro, colunaTiro;

    for (tiros = 0; tiros < 3; tiros++) {
        coordenadas_tiro(&linhaTiro, &colunaTiro, linha, coluna);
        printf("\nUm tiro atingiu a posição [%d][%d]!", linhaTiro, colunaTiro);
        escreva_mapa_arquivo_tiro (nome, linhaTiro, colunaTiro);

        situation = identifica_alvo_atingido(mapa, linhaTiro, colunaTiro, linha, coluna);
        if (situation == GAME_OVER_BY_ATTACK) situation_critical = situation; 

    }
    escreva_mapa_tela (mapa, linha, coluna);
    return situation_critical;
}

int identifica_alvo_atingido (char **mapa, int linhaTiro, int colunaTiro, int linha, int coluna) {
    int contador = 1;

    if(mapa[linhaTiro][colunaTiro] == '.')
    {
        printf("\nNada foi atingido!");
        mapa[linhaTiro][colunaTiro] = '=';
        return SAFE_MOVE;
    }
    if(mapa[linhaTiro][colunaTiro] == 'T')
    {
        printf("\nUm tiro atingiu uma das posicoes passadas por seu barco! Sorte que seu barco nao estava la nessa rodada!");
        mapa[linhaTiro][colunaTiro] = '+';
        return SAFE_MOVE;
    }

    if(mapa[linhaTiro][colunaTiro] == 'B')
    {
        printf("\nQue pena! Seu barco foi atingido por um tiro e afundou!");
        mapa[linhaTiro][colunaTiro] = '!';
        return GAME_OVER_BY_ATTACK;
    }
    if(mapa[linhaTiro][colunaTiro] == '=' || mapa[linhaTiro][colunaTiro] == '*' || mapa[linhaTiro][colunaTiro] == '+')
    {
        printf("\nNada foi atingido!");
        return SAFE_MOVE;
    }    
    if(mapa[linhaTiro][colunaTiro] == 'S' || mapa[linhaTiro][colunaTiro] == 'D' || mapa[linhaTiro][colunaTiro] == 'C' ||
       mapa[linhaTiro][colunaTiro] == 'P' || mapa[linhaTiro][colunaTiro] == 'H')
    {
        if(mapa[linhaTiro][colunaTiro] == 'S')
        {
            printf("\nUm SUBMARINO foi totalmente destruido!");
            mapa[linhaTiro][colunaTiro] = '*';
        }

        if(mapa[linhaTiro][colunaTiro] == 'D')
        {
            printf("\nUm DESTROYER foi totalmente destruido!");
            afunda_destroyer(mapa, linhaTiro, colunaTiro, linha - 1, coluna - 1, &contador);
        }

        if(mapa[linhaTiro][colunaTiro] == 'C')
        {
            printf("\nUm CRUZADOR foi totalmente destruido!");
            afunda_cruzador (mapa, linhaTiro, colunaTiro, linha - 1, coluna - 1, &contador);
        }

        if(mapa[linhaTiro][colunaTiro] == 'P')
        {
            printf("\nUm PORTA-AVIAO foi totalmente destruido!");
            //contador = 5;
            afunda_porta_aviao (mapa, linhaTiro, colunaTiro, linha - 1, coluna - 1, &contador);
        }

        if(mapa[linhaTiro][colunaTiro] == 'H')
        {
            printf("\nUm tiro atingiu um HIDRO-AVIAO! O mesmo foi totalmente destruido!");
            //contador = 3;
            afunda_hidro_aviao(mapa, linhaTiro, colunaTiro, linha - 1, coluna - 1, &contador);
        }
    }
    return SAFE_MOVE;
}

void afunda_destroyer(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';

    while( (*contador) < 2 )
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha - 1][coluna - 1] == 'D')
            {
                (*contador)++;
                mapa[linha - 1][coluna - 1] = '*';
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna - 1] == 'D')
            {
                (*contador)++;
                mapa[linha][coluna - 1] = '*';
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha + 1][coluna - 1] == 'D')
            {
                (*contador)++;
                mapa[linha + 1][coluna - 1] = '*';
            }
        }

        if(linha != 0)
        {
            if(mapa[linha - 1][coluna] == 'D')
            {
                (*contador)++;
                mapa[linha - 1][coluna] = '*';
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha + 1][coluna] == 'D')
            {
                (*contador)++;
                mapa[linha + 1][coluna] = '*';
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha - 1][coluna + 1] == 'D')
            {
                (*contador)++;
                mapa[linha - 1][coluna + 1] = '*';
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna + 1] == 'D')
            {
                (*contador)++;
                mapa[linha][coluna + 1] = '*';
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha + 1][coluna + 1] == 'D')
            {
                (*contador)++;
                mapa[linha + 1][coluna + 1] = '*';
            }
        }
    }
}


void afunda_cruzador(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';
    while( (*contador) < 4 )
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha - 1][coluna - 1] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha - 1, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna - 1] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha + 1][coluna - 1] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha + 1, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != 0)
        {
            if(mapa[linha - 1][coluna] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha - 1, coluna, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha + 1][coluna] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha + 1, coluna, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha - 1][coluna + 1] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha - 1, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna + 1] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha + 1][coluna + 1] == 'C')
            {
                (*contador)++;
                afunda_cruzador(mapa, linha + 1, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }
        break;
    }
}
void afunda_porta_aviao(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';

    while( (*contador) < 5 )
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha - 1][coluna - 1] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha - 1, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna - 1] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha + 1][coluna - 1] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha + 1, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != 0)
        {
            if(mapa[linha - 1][coluna] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha - 1, coluna, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha + 1][coluna] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha + 1, coluna, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha - 1][coluna + 1] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha - 1, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna + 1] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha + 1][coluna + 1] == 'P')
            {
                (*contador)++;
                afunda_porta_aviao(mapa, linha + 1, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }
        break;
    }
}

void afunda_hidro_aviao(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';

    while( (*contador) < 3 )
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha - 1][coluna - 1] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha - 1, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna - 1] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha + 1][coluna - 1] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha + 1, coluna - 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != 0)
        {
            if(mapa[linha - 1][coluna] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha - 1, coluna, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha + 1][coluna] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha + 1, coluna, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha - 1][coluna + 1] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha - 1, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna + 1] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha + 1][coluna + 1] == 'H')
            {
                (*contador)++;
                afunda_hidro_aviao(mapa, linha + 1, coluna + 1, rowMax, columnMax, &(*contador) );
            }
        }
        break;
    }
}
