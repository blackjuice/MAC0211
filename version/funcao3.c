#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 128
#define EXIT_FAILURE_MALLOC -1

void freeImagemGame(char **mapa, int linha);
void escreva_mapa_arquivo (char *nomeArquivo, char **mapa, int linha, int coluna);
void escreva_mapa_tela(char **M, int linha, int coluna);
char ** leia_mapa(char *nomeArquivo, int *linha, int *coluna);
char ** mallocImagemGame(int *linha, int *coluna);

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

char **
leia_mapa(char *nomeArquivo, int *linha, int *coluna)
{
	FILE *fEntrada;
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

void escreva_mapa_tela(char **M, int linha, int coluna)
{
    int 		i, j;

    printf("\n\n");
	for(i = 0; i < linha; i++) {
	    for(j = 0; j < coluna ; j++) {
    		printf("-");
    	}
        printf("\n");
    }
}

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

void
freeImagemGame(char **mapa, int linha)
{
	int 		i;
	for (i = 0; i < linha; i++)
		free (mapa[i]);
	free (mapa);
}

//int main(int argc, char *argv[])
int main()
{
	int 		linha = 0, coluna = 0;
	char 		**mapa;
	char 		saida_arquivo[MAX_NOME];
	char 		input[MAX_NOME];

	//char input[35] = "test.txt";
	//char saida_arquivo[20] = "result.txt";
	
	printf("Digite o caminho do arquivo mapa a ser utilizado: ");
	scanf("%s", input);
	printf("Digite o nome do arquivo que conterá toda a saída do programa: ");
	scanf("%s", saida_arquivo);

	/* 2. carregue o mapa do arquivo*/
	mapa = leia_mapa (input, &linha, &coluna);
	escreva_mapa_tela(mapa, linha, coluna);

	escreva_mapa_arquivo (saida_arquivo, mapa, linha, coluna);
	freeImagemGame(mapa, linha);
	return 0;
}
