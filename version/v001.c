#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 128
#define EXIT_FAILURE_MALLOC -1

typedef struct imagemGame ImagemGame;
struct imagemGame {
	int  	linha;
	int 	coluna;
	char 	info;
};

static void 	*mallocSafe (size_t nbytes);
ImagemGame 		*mallocImagemGame(int linha, int coluna);
ImagemGame 		* leia_mapa(char *nomeArquivo);
void 			freeImagemGame(ImagemGame *mapa);
void 			escreva_mapa_tela(ImagemGame *mapa);

/*-------------------------------------------------------------*/ 
/*  mallocSafe 

   O parâmetro de mallocSafe é do tipo size_t.  
   Em muitos computadores, size_t é equivalente a unsigned int.  
   A função mallocSafe não está em nenhuma biblioteca e é desconhecida 
   fora destas notas de aula. 
   Ela é apenas uma abreviatura conveniente.

   Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
 */
static void 
*mallocSafe (size_t nbytes)
{
   void *ptr;

   ptr = malloc (nbytes);
   if (ptr == NULL) {
     fprintf (stderr, "Socorro! malloc devolveu NULL!\n");
     exit (EXIT_FAILURE_MALLOC);
   }
   return ptr;
}

ImagemGame *
mallocImagemGame(int linha, int coluna)
{
  ImagemGame *mapa;
  mapa = mallocSafe (sizeof(ImagemGame));
  return mapa;
}

ImagemGame * leia_mapa(char *nomeArquivo)
{
	FILE *fEntrada;
	int 			linha;
	int 			coluna;  
	char 			c;
	ImagemGame 		*mapa;

	fEntrada = fopen(nomeArquivo, "r");
	if (fEntrada == NULL) 
	  {
	    fprintf(stderr, "ERRO! Nao consegui abrir o arquivo '%s'\n", nomeArquivo);
	    exit(EXIT_FAILURE);
	  }
	fprintf(stdout, "Arquivo '%s' com a imagem foi aberto.\n", nomeArquivo);

	/* inicio da leitura do arquivo */
	fscanf(fEntrada, "%d ", &linha);
	fscanf(fEntrada, "%d\n", &coluna) ;
	mapa = mallocImagemGame(linha, coluna);

	mapa->linha  = linha;
	mapa->coluna = coluna;

	for(linha = 0; linha < mapa->linha; linha++) {
	    for(coluna = 0; coluna < mapa->coluna; coluna++) {
	        fscanf(fEntrada, " %c", &mapa->info);
	        printf("%c", mapa->info);
	    }
	    printf("\n");
	}

	fclose(fEntrada);
	fprintf(stdout, "A imagem foi carregada e o "
	          		"arquivo '%s' foi fechado.\n", nomeArquivo);
	return (mapa);
}

void escreva_mapa_tela(ImagemGame *mapa)
{
    int linha, coluna;

    printf("\n\n");
    for(coluna = 0; coluna < mapa->coluna; coluna++) {
    	for(linha = 0; linha < mapa->linha; linha++) {

    		printf("-");
    	}
        printf("\n");
    }
}

void
freeImagemGame(ImagemGame *mapa)
{
	int i;
	for (i = 0; i < mapa->coluna; i++)
	free (mapa);
	mapa = NULL;
}

//int main(int argc, char *argv[])
int main()
{
	ImagemGame *mapa = NULL;
	char nome[MAX_NOME];

	/* 1. pegue o nome do arquivo com o mapa da entrada padrao */
	printf("Digite o caminho do arquivo mapa a ser utilizado: ");
	scanf("%s", nome);
	/* 2. carregue o mapa do arquivo*/
	mapa = leia_mapa (nome);
	escreva_mapa_tela(mapa);

	freeImagemGame(mapa);
	return 0;
}
