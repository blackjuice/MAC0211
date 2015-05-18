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

    char matriz[5][8];

    for(linha = 0; linha<x; linha++)
    {
        for(coluna = 0; coluna<y; coluna++)
        {
            matriz[linha][coluna] = fgetc(f1);
            printf("\nmatriz[%d][%d] = %c",linha,coluna,matriz[linha][coluna]);
        }
        c = fgetc(f1);
    }

    printf("\n\n--------------------- MATRIZ ---------------------\n\n\n");
    for(linha = 0; linha<x; linha++)
    {
        printf("                     ");
        for(coluna = 0; coluna<y; coluna++)
        {
            printf("%c",matriz[linha][coluna]);
        }
        printf("\n");
    }
    printf("\n\n--------------------------------------------------\n\n");
}

int main()
{
    leiaMapa();
	return 0;
}
