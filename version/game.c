#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME            128
#define SAFE_MOVE           0
#define GAME_OVER_BY_COLLISION        10
#define GAME_OVER_BY_ATTACK           11
#define GAME_OVER_SUCCESS             12

/**************************************
 *      Declaracao das funcoes
 **************************************/

char ** leia_mapa                   (char *nomeArquivo, int *linha, int *coluna);
char ** mallocImagemGame            (int *linha, int *coluna);
void    escreva_mapa_tela           (char **mapa, int linha, int coluna);
void    escreva_mapa_arquivo        (char *nomeArquivo, char **mapa, int linha, int coluna);
void    posiciona_barco             (char **mapa, int *coluna);
int     rema_barco                  (char **mapa, int *linAtual, int *colAtual, int rowMax, int columnMax);
void    freeImagemGame              (char **mapa, int linha);

int     sorteia                     (int k);
void    coordenadas_tiro            (int *linhaTiro, int *colunaTiro ,int linha, int coluna);
int     dispara_tiros               (char **mapa, int linha, int coluna);
int     identifica_alvo_atingido    (char **mapa, int linhaTiro, int colunaTiro, int linha, int coluna);

void afunda_destroyer(char **mapa, int linha, int coluna, int rowMax, int columnMax, int contador);

/**************************************
 *      Inicio da main()
 **************************************/
int main()
{
	int        linha = 0, coluna = 0;
	char       **mapa;

	int        linBarco = 0, colBarco = 0;
	/*
	char 		saida_arquivo[MAX_NOME];
	char 		input[MAX_NOME];

	printf("Digite o caminho do arquivo mapa a ser utilizado: ");
	scanf("%s", input);
	printf("Digite o nome do arquivo que conterá toda a saída do programa: ");
	scanf("%s", saida_arquivo);

	*/
	int        situation = GAME_OVER_SUCCESS;

	//char input[35] = "test.txt";
	char input[35] = "test1.txt";
	char saida_arquivo[20] = "result.txt";

	/* 2. carregue o mapa do arquivo*/
	mapa = leia_mapa (input, &linha, &coluna);
	escreva_mapa_tela(mapa, linha, coluna);
	posiciona_barco	(mapa, &colBarco);
	escreva_mapa_tela(mapa, linha, coluna);

	// programa encerra se barco alcanca a base do mapa
	while(linBarco != linha - 1) {

		situation = rema_barco (mapa, &linBarco, &colBarco, linha, coluna);
		if (situation == GAME_OVER_BY_COLLISION) break;

		situation = dispara_tiros (mapa, linha, coluna);
        if (situation == GAME_OVER_BY_ATTACK) break;
		escreva_mapa_tela(mapa, linha, coluna);
		escreva_mapa_arquivo (saida_arquivo, mapa, linha, coluna);
	


	}

    if (situation == GAME_OVER_BY_ATTACK)
        printf("ataque!\n");
    if (situation == GAME_OVER_BY_COLLISION)
        printf("colisao!\n");
    else
        printf("SUCESSO\n");

    printf( "  ______________________\n"
            " |                      |\n"
            " |      GAME OVER       |\n"
            " |______________________|\n\n");
	freeImagemGame(mapa, linha);
	return 0;
}



/**************************************
 *      Inicio das funcoes
 **************************************/

/* alocacao da matriz que contera o mapa */
char **
mallocImagemGame(int *linha, int *coluna)
{
	int 			i;
	char 			**M;
	
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
	fprintf(stdout, "A mapa '%s' carregado com sucesso.\n\n", nomeArquivo);

	/* inicio da leitura do arquivo */
	fscanf(fEntrada, "%d ", &(*linha));
	fscanf(fEntrada, "%d\n", &(*coluna)) ;
	mapa = mallocImagemGame((&(*linha)), (&(*coluna)) );

	for(i = 0; i < (*linha); i++) {
	    for(j = 0; j < (*coluna); j++) {
	        fscanf(fEntrada, " %c", &c);
	        mapa[i][j] = c;
	        //printf("%c", mapa[i][j]);
	    }
	    //printf("\n");
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
	//fSaida = fopen(nome, "w");
	fSaida = fopen(nome, "a+");
	if (fSaida == NULL) 
	{
		fprintf(stderr, "ERRO! arquivo '%s' nao pode ser criado\n", nome);
		exit(EXIT_FAILURE);
	}

	fprintf(fSaida, "%d " , linha );
	fprintf(fSaida, "%d\n\n", coluna );

	for(i = 0; i < linha; i++) {
	    for(j = 0; j < coluna; j++) {
			fprintf(fSaida, "%c", mapa[i][j]);
		}
		fprintf(fSaida, "\n");
	}

	fclose(fSaida);
	fprintf(stdout, "A imagem foi salva no arquivo: '%s'\n", nome);
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
posiciona_barco	(char **mapa, int *coluna)
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
0 - se a posicao for invalida
1 - se a posicao for valida
2 - se houve colisao
*/
int 
rema_barco (char **mapa, int *linAtual, int *colAtual, int rowMax, int columnMax) {
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
            "      Direcao:");

    scanf(" %c", &escolha);

    if(escolha == 'c') {
        // estamos no topo do mapa. Subir sera invalido.
        if((*linAtual) == 0)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual) - 1][(*colAtual)] == 'S' || mapa[(*linAtual) - 1][(*colAtual)] == 'D' || 
                mapa[(*linAtual) - 1][(*colAtual)] == 'C' || mapa[(*linAtual) - 1][(*colAtual)] == 'P' ||
                mapa[(*linAtual) - 1][(*colAtual)] == 'H')
            { 
                mapa[(*linAtual)][(*colAtual)] 	= 'T';
                (*linAtual)--;
                return 2;
            }
        else {
                mapa[(*linAtual)][(*colAtual)] 	= 'T';
                mapa[(*linAtual) - 1][(*colAtual)] = 'B';
                (*linAtual)--;
                return 1;
        }
    }

    if(escolha == 'b') {
    	// estamos na base do mapa. Descer sera invalido
        if((*linAtual) == rowMax)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual) + 1][(*colAtual)] == 'S' || mapa[(*linAtual) + 1][(*colAtual)] == 'D' || 
                mapa[(*linAtual) + 1][(*colAtual)] == 'C' || mapa[(*linAtual) + 1][(*colAtual)] == 'P' || 
                mapa[(*linAtual) + 1][(*colAtual)] == 'H')
            {
                mapa[(*linAtual)][(*colAtual)] 	= 'T';
                (*linAtual)++;
                return 2;
            }
        else {
                mapa[(*linAtual)][(*colAtual)] 	= 'T';
                mapa[(*linAtual) + 1][(*colAtual)] = 'B';
                (*linAtual)++;
                return 1;
        }
    }

    if(escolha == 'e') {
        // estamos na borda esquerda. Ir para esquerda sera invalido
        if((*colAtual) == 0)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual)][(*colAtual) - 1] == 'S' || mapa[(*linAtual)][(*colAtual) - 1] == 'D' || 
                mapa[(*linAtual)][(*colAtual) - 1] == 'C' || mapa[(*linAtual)][(*colAtual) - 1] == 'P' ||
                mapa[(*linAtual)][(*colAtual) - 1] == 'H')
            {
                mapa[(*linAtual)][(*colAtual)] 	= 'T';
                (*colAtual)--;
                return 2;
            }
        else {
                mapa[(*linAtual)][(*colAtual)] = 'T';
                mapa[(*linAtual)][(*colAtual) - 1] = 'B';
                (*colAtual)--;
                return 1;
        }
    }

    if(escolha == 'd') {
        // estamos na borda direita. Ir para direita sera invalido
        if((*colAtual) == columnMax)
            printf("\nERRO! Siga para uma direção válida!\n");

        else if(mapa[(*linAtual)][(*colAtual) + 1] == 'S' || mapa[(*linAtual)][(*colAtual) + 1] == 'D' ||
                mapa[(*linAtual)][(*colAtual) + 1] == 'C' || mapa[(*linAtual)][(*colAtual) + 1] == 'P' ||
                mapa[(*linAtual)][(*colAtual) + 1] == 'H')
            {
                mapa[(*linAtual)][(*colAtual)] 	= 'T';
                (*colAtual)++;
                return 2;
            }
        else {
                mapa[(*linAtual)][(*colAtual)] = 'T';
                mapa[(*linAtual)][(*colAtual) + 1] = 'B';
                (*colAtual)++;
                return 1;
        }
    }
    return 0;
}

int sorteia (int k) {
    return (int)(1 + (rand() / (RAND_MAX + 1.0)) * k);
}

void coordenadas_tiro (int *linhaTiro, int *colunaTiro ,int linha, int coluna) {
    *linhaTiro 	= sorteia(linha - 1);
    *colunaTiro = sorteia(coluna - 1);
}

int dispara_tiros (char **mapa, int linha, int coluna)
{
    int    contador;
    int    situation_critical;
    int    linhaTiro, colunaTiro;

    for (contador = 0; contador < 3; contador++) {
        coordenadas_tiro(&linhaTiro, &colunaTiro, linha, coluna);
        situation_critical = identifica_alvo_atingido(mapa, linhaTiro, colunaTiro, linha, coluna);
        //identifica_alvo_atingido(mapa, &linhaTiro, &colunaTiro, linha, coluna);
        printf("\nUm tiro atingiu a posição [%d][%d]!", linhaTiro, colunaTiro);

    }
    escreva_mapa_tela (mapa, linha, coluna);
    return situation_critical;
}

int identifica_alvo_atingido (char **mapa, int linhaTiro, int colunaTiro, int linha, int coluna) {
    if(mapa[linhaTiro][colunaTiro] == '.')
    {
        printf("\nSorte que so tinha agua no local!\n");
        mapa[linhaTiro][colunaTiro] = '=';
        return SAFE_MOVE;
    }
    if(mapa[linhaTiro][colunaTiro] == 'T')
    {
        printf("\nUm tiro atingiu uma das posicoes passadas por seu barco! Sorte que seu barco nao estava la nessa rodada!\n");
        mapa[linhaTiro][colunaTiro] = '+';
        return SAFE_MOVE;
    }
    if(mapa[linhaTiro][colunaTiro] == 'B')
    {
        printf("\nQue pena! Seu barco foi atingido por um tiro e afundou!\n");
        mapa[linhaTiro][colunaTiro] = '!';
        return GAME_OVER_BY_ATTACK;
    }
    if(mapa[linhaTiro][colunaTiro] == 'S' || mapa[linhaTiro][colunaTiro] == 'D' || mapa[linhaTiro][colunaTiro] == 'C' ||
       mapa[linhaTiro][colunaTiro] == 'P' || mapa[linhaTiro][colunaTiro] == 'H')
    {
        if(mapa[linhaTiro][colunaTiro] == 'S')
        {
            printf("\nUm tiro atingiu um submarino! O mesmo foi totalmente destruido!\n");
            mapa[linhaTiro][colunaTiro] = '*';
        }

        if(mapa[linhaTiro][colunaTiro] == 'D')
        {
            printf("\nUm tiro atingiu um destroyer! O mesmo foi totalmente destruido!\n");
            afunda_destroyer(mapa, linhaTiro, colunaTiro, linha - 1, coluna - 1, 1);
        }

        if(mapa[linhaTiro][colunaTiro] == 'C')
        {
            printf("\nUm tiro atingiu um cruzador! O mesmo foi totalmente destruido!\n");
            //mapa = afunda_cruzador(mapa,linhaTiro,colunaTiro);
        }

        if(mapa[linhaTiro][colunaTiro] == 'P')
        {
            printf("\nUm tiro atingiu um porta-aviao! O mesmo foi totalmente destruido!\n");
            //mapa = afunda_porta_aviao(mapa,linhaTiro,colunaTiro);
        }

        if(mapa[linhaTiro][colunaTiro] == 'H')
        {
            printf("\nUm tiro atingiu um hidro-aviao! O mesmo foi totalmente destruido!\n");
            //mapa = afunda_hidro_aviao(mapa,linhaTiro,colunaTiro);
        }
    }
    return SAFE_MOVE;
}

void afunda_destroyer(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';

    while(contador < 2)
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha - 1][coluna - 1] == 'D')
            {
                contador++;
                mapa[linha - 1][coluna - 1] = '*';
                afunda_destroyer(mapa, linha - 1, coluna - 1, rowMax, columnMax, contador);
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna - 1] == 'D')
            {
                contador++;
                mapa[linha][coluna - 1] = '*';
                afunda_destroyer(mapa, linha, coluna - 1, rowMax, columnMax, contador);
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha + 1][coluna - 1] == 'D')
            {
                contador++;
                mapa[linha + 1][coluna - 1] = '*';
                afunda_destroyer(mapa, linha + 1,coluna - 1, rowMax, columnMax, contador);
            }
        }

        if(linha != 0)
        {
            if(mapa[linha - 1][coluna] == 'D')
            {
                contador++;
                mapa[linha - 1][coluna] = '*';
                afunda_destroyer(mapa, linha - 1, coluna, rowMax, columnMax, contador);
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha + 1][coluna] == 'D')
            {
                contador++;
                mapa[linha + 1][coluna] = '*';
                afunda_destroyer(mapa,linha + 1, coluna, rowMax, columnMax, contador);
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha - 1][coluna + 1] == 'D')
            {
                contador++;
                mapa[linha - 1][coluna + 1] = '*';
                afunda_destroyer(mapa, linha - 1, coluna + 1, rowMax, columnMax, contador);
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna + 1] == 'D')
            {
                contador++;
                mapa[linha][coluna + 1] = '*';
                afunda_destroyer(mapa, linha, coluna + 1, rowMax, columnMax, contador);
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha + 1][coluna + 1] == 'D')
            {
                contador++;
                mapa[linha + 1][coluna + 1] = '*';
                afunda_destroyer(mapa, linha + 1, coluna + 1, rowMax, columnMax, contador);
            }
        }
    }
}

void afunda_cruzador(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';

    while(contador < 4)
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha-1][coluna-1] == 'C')
            {
                contador++;
                mapa[linha-1][coluna-1] = '*';
                afunda_cruzador(mapa,linha-1,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna-1] == 'C')
            {
                contador++;
                mapa[linha][coluna-1] = '*';
                afunda_cruzador(mapa,linha,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha+1][coluna-1] == 'C')
            {
                contador++;
                mapa[linha+1][coluna-1] = '*';
                afunda_cruzador(mapa,linha+1,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(linha != 0)
        {
            if(mapa[linha-1][coluna] == 'C')
            {
                contador++;
                mapa[linha-1][coluna] = '*';
                afunda_cruzador(mapa,linha-1,coluna,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha+1][coluna] == 'C')
            {
                contador++;
                mapa[linha+1][coluna] = '*';
                afunda_cruzador(mapa,linha+1,coluna,rowMax,columnMax,contador);
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha-1][coluna+1] == 'C')
            {
                contador++;
                mapa[linha-1][coluna+1] = '*';
                afunda_cruzador(mapa,linha-1,coluna+1,rowMax,columnMax,contador);
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna+1] == 'C')
            {
                contador++;
                mapa[linha][coluna+1] = '*';
                afunda_cruzador(mapa,linha,coluna+1,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha+1][coluna+1] == 'C')
            {
                contador++;
                mapa[linha+1][coluna+1] = '*';
                afunda_cruzador(mapa,linha+1,coluna+1,rowMax,columnMax,contador);
            }
        }
    }
}

void afunda_porta_aviao(char **mapa, int linha, int coluna, int rowMax, int columnMax, int *contador)
{
    mapa[linha][coluna] = '*';

    while(contador < 5)
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha-1][coluna-1] == 'P')
            {
                contador++;
                mapa[linha-1][coluna-1] = '*';
                afunda_porta_aviao(mapa,linha-1,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna-1] == 'P')
            {
                contador++;
                mapa[linha][coluna-1] = '*';
                afunda_porta_aviao(mapa,linha,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha+1][coluna-1] == 'P')
            {
                contador++;
                mapa[linha+1][coluna-1] = '*';
                afunda_porta_aviao(mapa,linha+1,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(linha != 0)
        {
            if(mapa[linha-1][coluna] == 'P')
            {
                contador++;
                mapa[linha-1][coluna] = '*';
                afunda_porta_aviao(mapa,linha-1,coluna,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha+1][coluna] == 'P')
            {
                contador++;
                mapa[linha+1][coluna] = '*';
                afunda_porta_aviao(mapa,linha+1,coluna,rowMax,columnMax,contador);
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha-1][coluna+1] == 'P')
            {
                contador++;
                mapa[linha-1][coluna+1] = '*';
                afunda_porta_aviao(mapa,linha-1,coluna+1,rowMax,columnMax,contador);
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna+1] == 'P')
            {
                contador++;
                mapa[linha][coluna+1] = '*';
                afunda_porta_aviao(mapa,linha,coluna+1,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha+1][coluna+1] == 'P')
            {
                contador++;
                mapa[linha+1][coluna+1] = '*';
                afunda_porta_aviao(mapa,linha+1,coluna+1,rowMax,columnMax,contador);
            }
        }
    }
}
