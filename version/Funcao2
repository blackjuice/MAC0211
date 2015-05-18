#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef char *string;

char** leiaMapa()
{
    string coordenadax,coordenaday;
    char s[100];
	char *mapaInicial;
	char c;
	int linha,coluna,x,y;

	mapaInicial = malloc (strlen(mapaInicial)*sizeof(char));

	FILE *f1;

	printf("Digite o caminho do arquivo mapa a ser utilizado: ");
	scanf("%s",mapaInicial);

	f1 = fopen(mapaInicial,"r");

	if (f1 == NULL)
   	{
		printf ("\nHouve um erro ao abrir o arquivo.\n\n");
		return leiaMapa();
	}

	fgets(s,100,f1);

	coordenadax = strtok(s," ");
    x = atoi(coordenadax);

    coordenaday = strtok(NULL,"\n");
    y = atoi(coordenaday);

    char mapa[5][8];

    for(linha = 0; linha<x; linha++)
    {
        for(coluna = 0; coluna<y; coluna++)
        {
            mapa[linha][coluna] = fgetc(f1);
            printf("\nmatriz[%d][%d] = %c",linha,coluna,mapa[linha][coluna]);
        }
        c = fgetc(f1);
        if(c = '\n')
            printf("\nC EH IGUAL A BARRA ENE");
    }

    printf("\n\n--------------------- MATRIZ ---------------------\n\n\n");
    for(linha = 0; linha<x; linha++)
    {
        printf("                     ");
        for(coluna = 0; coluna<y; coluna++)
        {
            printf("%c",mapa[linha][coluna]);
        }
        printf("\n");
    }
    printf("\n\n--------------------------------------------------\n\n");
}

void escreva_mapa_tela(char mapa_tela[5][8])
{
    int linha,coluna;

    printf("\n\n  ");
    for(coluna = 0; coluna<8; coluna++)
    {
        printf("%d",coluna+1);
    }

    printf("\n");
    for(linha = 0; linha<5; linha++)
    {
        printf("%d ",linha+1);
        for(coluna = 0; coluna<8; coluna++)
        {
            printf("%c",mapa_tela[linha][coluna]);
        }
        printf("\n");
    }
}

int main()
{
    int linha,coluna;
    char mapa_tela[5][8];
    leiaMapa();


    for(linha = 0; linha<5; linha++)
    {
        for(coluna = 0; coluna<8; coluna++)
        {
            mapa_tela[linha][coluna] = '-';
        }
    }

    for(linha = 0; linha<5; linha++)
    {
        for(coluna = 0; coluna<8; coluna++)
        {
            printf("%c",mapa_tela[linha][coluna]);
        }
        printf("\n");
    }
    escreva_mapa_tela(mapa_tela);

	return 0;
}
