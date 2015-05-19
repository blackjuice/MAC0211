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
int             posiciona_barco(ImagemGame *mapa);
int             rema_barco(ImagemGame *mapa, int linAtual, int colAtual);

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
    for(linha = 0; linha < mapa->linha; linha++) {
    	for(coluna = 0; coluna < mapa->coluna; coluna++) {

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

int posiciona_barco(ImagemGame *mapa)
{
    int pos;

    printf("\n\nDigite a coluna da primeira linha do mapa em que deseja posicionar o barco: ");
    scanf("%d",&pos);

    if(mapa[0][pos] != '.')
    {
        printf("\nPosicao invalida!");
        return posiciona_barco(mapa);
    }

    mapa[0][pos] = 'B';
    return pos;
}
/*

retorna:

1 - se o barco remar para cima, e essa posicao estiver disponivel
2 - se o barco remar para baixo, e essa posicao estiver disponivel
3 - se o barco remar para esquerda, e essa posicao estiver disponivel
4 - se o barco remar para direita, e essa posicao estiver disponivel
5 - se o barco colidir com alguma embarcacao, perdendo o jogo

*/
int rema_barco (ImagemGame *mapa, int linAtual, int colAtual)
{
    char escolha;

    printf("Digite a direcao escolhida para o barco remar: \n\n(c) para cima\n(b) para baixo\n(e) para esquerda\n(d) para direita\n\n");
    scanf("%c",&escolha);

    if(escolha == 'c')
    {
        if(mapa[linAtual-1][colAtual] == NULL)
        {
            printf("\nEssa posicao nao existe no mapa!\n");
            return rema_barco(mapa,linAtual,colAtual);
        }
        else if(mapa[linAtual-1][colAtual] == 'S' || mapa[linAtual-1][colAtual] == 'D' || mapa[linAtual-1][colAtual] == 'C' ||
           mapa[linAtual-1][colAtual] == 'P' || mapa[linAtual-1][colAtual] == 'H')
            return 5;
        else
        {
            mapa[linAtual][colAtual] = 'T';
            mapa[linAtual-1][colAtual] = 'B';
            return 1;
        }
    }

    if(escolha == 'b')
    {
        if(mapa[linAtual+1][colAtual] == NULL)
        {
            printf("\nEssa posicao nao existe no mapa!\n");
            return rema_barco(mapa,linAtual,colAtual);
        }
        else if(mapa[linAtual+1][colAtual] == 'S' || mapa[linAtual+1][colAtual] == 'D' || mapa[linAtual+1][colAtual] == 'C' ||
           mapa[linAtual+1][colAtual] == 'P' || mapa[linAtual+1][colAtual] == 'H')
            return 5;
        else
        {
            mapa[linAtual][colAtual] = 'T';
            mapa[linAtual+1][colAtual] = 'B';
            return 2;
        }
    }

    if(escolha == 'e')
    {
        if(mapa[linAtual][colAtual-1] == NULL)
        {
            printf("\nEssa posicao nao existe no mapa!\n");
            return rema_barco(mapa,linAtual,colAtual);
        }
        else if(mapa[linAtual][colAtual-1] == 'S' || mapa[linAtual][colAtual-1] == 'D' || mapa[linAtual][colAtual-1] == 'C' ||
           mapa[linAtual][colAtual-1] == 'P' || mapa[linAtual][colAtual-1] == 'H')
            return 5;
        else
        {
            mapa[linAtual][colAtual] = 'T';
            mapa[linAtual][colAtual-1] = 'B';
            return 3;
        }
    }

    if(escolha == 'd')
    {
        if(mapa[linAtual][colAtual+1] == NULL)
        {
            printf("\nEssa posicao nao existe no mapa!\n");
            return rema_barco(mapa,linAtual,colAtual);
        }
        else if(mapa[linAtual][colAtual+1] == 'S' || mapa[linAtual][colAtual+1] == 'D' || mapa[linAtual][colAtual+1] == 'C' ||
           mapa[linAtual][colAtual+1] == 'P' || mapa[linAtual][colAtual+1] == 'H')
            return 5;
        else
        {
            mapa[linAtual][colAtual] = 'T';
            mapa[linAtual][colAtual+1] = 'B';
            return 4;
        }
    }
}

int sorteia(int k)
{
    return (int)(1 + (rand() / (RAND_MAX + 1.0)) * k);
}

int coordenadas_tiro(int *linTiro, int nLinhas, int nColunas)
{
    *linTiro = sorteia(nLinhas);
    return sorteia(nColunas);
}

ImagemGame * afunda_destroyer(ImagemGame *mapa, int linha, int coluna)
{
    /*
    O tiro atingiu mapa[linha][coluna]
    transformar mapa[linha][coluna] em '*', detectar os outros componentes do destroyer e transforma-los em '*' tambem.
    */
}

ImagemGame * afunda_cruzador(ImagemGame *mapa, int linha, int coluna)
{
    /*
    O tiro atingiu mapa[linha][coluna]
    transformar mapa[linha][coluna] em '*', detectar os outros componentes do cruzador e transforma-los em '*' tambem.
    */
}

ImagemGame * afunda_porta_aviao(ImagemGame *mapa, int linha, int coluna)
{
    /*
    O tiro atingiu mapa[linha][coluna]
    transformar mapa[linha][coluna] em '*', detectar os outros componentes do porta-aviao e transforma-los em '*' tambem.
    */
}

ImagemGame * afunda_hidro_aviao(ImagemGame *mapa, int linha, int coluna)
{
    /*
    O tiro atingiu mapa[linha][coluna]
    transformar mapa[linha][coluna] em '*', detectar os outros componentes do hidro-aviao e transforma-los em '*' tambem.
    */
}

ImagemGame * identifica_alvo_atingido(ImagemGame *mapa)
{
    int *linhaTiro,colunaTiro;
    colunaTiro = coordenadas_tiro(linhaTiro,nLinhas,nColunas);

    printf("\nUm tiro atingiu o pixel [%d][%d]!",linhaTiro,colunaTiro);

    if(mapa[linhaTiro][colunaTiro] == '.')
    {
        printf("\nSorte que so tinha agua no local!\n");
        mapa[linhaTiro][colunaTiro] = '=';
    }
    if(mapa[linhaTiro][colunaTiro] == 'B')
    {
        printf("\nQue pena! Seu barco foi atingido por um tiro e afundou!\n");
        mapa[linhaTiro][colunaTiro] = '!';
    }
    if(mapa[linhaTiro][colunaTiro] == 'T')
    {
        printf("\nUm tiro atingiu uma das posicoes passadas por seu barco! Sorte que seu barco nao estava la nessa rodada!\n");
        mapa[linhaTiro][colunaTiro] = '+';
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
            mapa = afunda_destroyer(mapa,linhaTiro,colunaTiro);
        }

        if(mapa[linhaTiro][colunaTiro] == 'C')
        {
            printf("\nUm tiro atingiu um cruzador! O mesmo foi totalmente destruido!\n");
            mapa = afunda_cruzador(mapa,linhaTiro,colunaTiro);
        }

        if(mapa[linhaTiro][colunaTiro] == 'P')
        {
            printf("\nUm tiro atingiu um porta-aviao! O mesmo foi totalmente destruido!\n");
            mapa = afunda_porta_aviao(mapa,linhaTiro,colunaTiro);
        }

        if(mapa[linhaTiro][colunaTiro] == 'H')
        {
            printf("\nUm tiro atingiu um hidro-aviao! O mesmo foi totalmente destruido!\n");
            mapa = afunda_hidro_aviao(mapa,linhaTiro,colunaTiro);
        }
    }
}

// no caso do exemplo mapa1.txt, nLinhas = 5 e nColunas = 8
void dispara_tiros(ImagemGame *mapa, int nLinhas, int nColunas)
{
    int contador;
    contador = 0;

    while(contador < 3)
    {
        mapa = identifica_alvo_atingido(mapa);

    }
}

//int main(int argc, char *argv[])
int main()
{
	ImagemGame *mapa = NULL;
	char nome[MAX_NOME];
	int linAtual, colAtual,rodada;

	/* 1. pegue o nome do arquivo com o mapa da entrada padrao */
	printf("Digite o caminho do arquivo mapa a ser utilizado: ");
	scanf("%s", nome);
	/* 2. carregue o mapa do arquivo*/
	mapa = leia_mapa (nome);
	escreva_mapa_tela(mapa);

	linAtual = 0;
	colAtual = posiciona_barco(mapa);

	while(linAtual != 4 /* ultima linha do mapa, no caso do exemplo mapa1 */)
    {
        rodada = rema_barco(mapa,linAtual,colAtual);
        if(rodada == 5)
        {
            printf("\n\nQue pena! Seu barco colidiu com uma embarcacao e afundou!\n\n");
            return 0;
        }
        if(rodada == 1)
        {
            linAtual--;
        }
        if(rodada == 2)
        {
            linAtual++;
        }
        if(rodada == 3)
        {
            colAtual--;
        }
        if(rodada == 4)
        {
            colAtual++;
        }

        if(mapa[linAtual][colAtual] == '!')
        {
            printf("\n\nQue pena! Seu barco foi atingido por um tiro e afundou!\n\n");
            return 0;
        }
    }

	freeImagemGame(mapa);
	return 0;
}

