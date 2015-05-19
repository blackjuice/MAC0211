#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 128
#define EXIT_FAILURE_MALLOC -1

/**************************************
 * 			Declaracao das funcoes
 **************************************/

char ** leia_mapa					(char *nomeArquivo, int *linha, int *coluna);
char ** mallocImagemGame			(int *linha, int *coluna);
void 	escreva_mapa_tela			(char **M, int linha, int coluna);
void 	escreva_mapa_arquivo 		(char *nomeArquivo, char **mapa, int linha, int coluna);
void	posiciona_barco				(char **mapa, int coluna);
int 	rema_barco 					(char **mapa, int linha, int coluna, int linhaMax, int colunaMax);
void 	freeImagemGame				(char **mapa, int linha);




/**************************************
 * 			Inicio da main()
 **************************************/
int main()
{
	int 		linha = 0, coluna = 0;
	char 		**mapa;
	char 		saida_arquivo[MAX_NOME];
	char 		input[MAX_NOME];


	int x;
	//char input[35] = "test.txt";
	//char saida_arquivo[20] = "result.txt";
	
	printf("Digite o caminho do arquivo mapa a ser utilizado: ");
	scanf("%s", input);
	printf("Digite o nome do arquivo que conterá toda a saída do programa: ");
	scanf("%s", saida_arquivo);

	/* 2. carregue o mapa do arquivo*/
	mapa = leia_mapa (input, &linha, &coluna);
	escreva_mapa_tela(mapa, linha, coluna);


	posiciona_barco	(mapa, coluna);
	x = rema_barco (mapa, linha, coluna, linhaMax, colunaMax);




	escreva_mapa_arquivo (saida_arquivo, mapa, linha, coluna);
	freeImagemGame(mapa, linha);
	return 0;
}



/**************************************
 * 			Inicio das funcoes
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
	FILE 			*fEntrada;
	int 			i, j;
	char 			c;
	char 			**mapa;

	fEntrada = fopen(nomeArquivo, "r");
	if (fEntrada == NULL) 
	{
	    fprintf(stderr, "ERRO! Nao consegui abrir o arquivo '%s'\n", nomeArquivo);
	    exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Arquivo '%s' com a imagem foi aberto.\n", nomeArquivo);

	/* inicio da leitura do arquivo */
	fscanf(fEntrada, "%d ", &(*linha));
	fscanf(fEntrada, "%d\n", &(*coluna)) ;
	mapa = mallocImagemGame((&(*linha)), (&(*coluna)) );

	for(i = 0; i < (*linha); i++) {
	    for(j = 0; j < (*coluna); j++) {
	        fscanf(fEntrada, " %c", &c);
	        mapa[i][j] = c;
	        printf("%c", mapa[i][j]);
	    }
	    printf("\n");
	}

	fclose(fEntrada);
	fprintf(stdout, "A imagem foi carregada e o "
	          		"arquivo '%s' foi fechado.\n", nomeArquivo);
	return (mapa);
}

/* impressao da tela para o usuario */
void escreva_mapa_tela (char **M, int linha, int coluna)
{
    int 		i, j;

    printf("\n");
	for(i = 0; i < linha; i++) {
	    for(j = 0; j < coluna ; j++) {
    		printf("-");
    	}
        printf("\n");
    }
}

/* criacao de arquivo que contera os resultados do mapa */
void escreva_mapa_arquivo (char *nome, char **mapa, int linha, int coluna)
{
	FILE 		*fSaida;
	int 		i, j;

	/* criaremos um arquivo (w = write) */
	fSaida = fopen(nome, "w");
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
	int 	i;
	for (i = 0; i < linha; i++)
		free (mapa[i]);
	free (mapa);
}


void
posiciona_barco	(char **mapa, int coluna)
{
	int 	pos;

	printf("\n\nDigite a coluna da primeira linha do mapa em que deseja posicionar o barco");

	while (mapa[0][pos] == '.') {
	    printf("\nIndique uma posicao válida: ");
		scanf("%d",&pos);
	}

	mapa[0][pos] = 'B';
}

/*
retorna:
0 - se a posicao for invalida
1 - se o barco remar para cima, e essa posicao estiver disponivel
2 - se o barco remar para baixo, e essa posicao estiver disponivel
3 - se o barco remar para esquerda, e essa posicao estiver disponivel
4 - se o barco remar para direita, e essa posicao estiver disponivel
5 - se o barco colidir com alguma embarcacao, perdendo o jogo
*/
int 
rema_barco (char **mapa, int linha, int coluna, int linhaMax, int colunaMax) {
	char escolha;

    printf( "-----------------------"
    		"Direcao do barco:\n"
    		" 	(c) para cima\n"
    		"	(b) para baixo\n"
			"	(d) para direita\n"
    		"	(e) para esquerda\n"
    		"-----------------------\n");
    scanf("%c", &escolha);

    if(escolha == 'c') {
    	// estamos no topo do mapa. Subir sera invalido.
        if(linha == 0)
            printf("\nEssa posicao nao existe no mapa!\n");

        else if(mapa[linha - 1][coluna] == 'S' || mapa[linha - 1][coluna] == 'D' || 
        		mapa[linha - 1][coluna] == 'C' || mapa[linha - 1][coluna] == 'P' ||
           		mapa[linha - 1][coluna] == 'H') 	{ return 5; }
	    else {
	            mapa[linha][coluna] 	= 'T';
	            mapa[linha - 1][coluna] = 'B';
	            return 1;
	    }
    }

    if(escolha == 'b') {
    	// estamos na base do mapa. Descer sera invalido
        if(linha == linhaMax)
            printf("\nEssa posicao nao existe no mapa!\n");

        else if(mapa[linha + 1][coluna] == 'S' || mapa[linha + 1][coluna] == 'D' || 
        		mapa[linha + 1][coluna] == 'C' || mapa[linha + 1][coluna] == 'P' || 
           		mapa[linha + 1][coluna] == 'H') 	{ return 5; }
		else {
			    mapa[linha][coluna] 	= 'T';
			    mapa[linha + 1][coluna] = 'B';
			    return 2;
		}
    }

    if(escolha == 'e') {
    	// estamos na borda esquerda. Ir para esquerda sera invalido
        if(coluna == 0)
            printf("\nEssa posicao nao existe no mapa!\n");

        else if(mapa[linha][coluna - 1] == 'S' || mapa[linha][coluna - 1] == 'D' || 
        		mapa[linha][coluna - 1] == 'C' || mapa[linha][coluna - 1] == 'P' ||
				mapa[linha][coluna - 1] == 'H') 	{ return 5; }
	    else {
	            mapa[linha][coluna - 1] = 'T';
	            mapa[linha][coluna - 1] = 'B';
	            return 3;
	    }
    }

    if(escolha == 'd') {
    	// estamos na borda direita. Ir para direita sera invalido
        if(coluna == colunaMax)
            printf("\nEssa posicao nao existe no mapa!\n");

        else if(mapa[linha][coluna + 1] == 'S' || mapa[linha][coluna + 1] == 'D' ||
        		mapa[linha][coluna + 1] == 'C' || mapa[linha][coluna + 1] == 'P' ||
           		mapa[linha][coluna + 1] == 'H') 	{ return 5; }
		else {
	            mapa[linha][coluna + 1] = 'T';
	            mapa[linha][coluna + 1] = 'B';
	            return 4;
	    }
    }
    return 0;
}
