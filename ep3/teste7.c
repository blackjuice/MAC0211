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
#include <unistd.h>
#include "xwc.h"

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
 
    WINDOW *w1;
    MASK msk;

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
    Background = ReadPic(w1, "backgroundBasic1.xpm", NULL);
    for ( i = 0; i < sizeX; i++ ) {
        for ( j = 0; j < sizeY; j++ ) {
            PutPic(w1, Background, 0, 0, 100, 100, i, j);        
            j += 29;
        }
        i += 29;
    }


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
