void afunda_destroyer(char **mapa, int linha, int coluna, int rowMax, int columnMax, int contador)
{
    mapa[linha][coluna] = '*';

    while(contador < 2)
    {
        if(linha != 0 && coluna != 0)
        {
            if(mapa[linha-1][coluna-1] == 'D')
            {
                contador++;
                mapa[linha-1][coluna-1] = '*';
                afunda_destroyer(mapa,linha-1,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(coluna != 0)
        {
            if(mapa[linha][coluna-1] == 'D')
            {
                contador++;
                mapa[linha][coluna-1] = '*';
                afunda_destroyer(mapa,linha,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax && coluna != 0)
        {
            if(mapa[linha+1][coluna-1] == 'D')
            {
                contador++;
                mapa[linha+1][coluna-1] = '*';
                afunda_destroyer(mapa,linha+1,coluna-1,rowMax,columnMax,contador);
            }
        }

        if(linha != 0)
        {
            if(mapa[linha-1][coluna] == 'D')
            {
                contador++;
                mapa[linha-1][coluna] = '*';
                afunda_destroyer(mapa,linha-1,coluna,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax)
        {
            if(mapa[linha+1][coluna] == 'D')
            {
                contador++;
                mapa[linha+1][coluna] = '*';
                afunda_destroyer(mapa,linha+1,coluna,rowMax,columnMax,contador);
            }
        }

        if(linha != 0 && coluna != columnMax)
        {
            if(mapa[linha-1][coluna+1] == 'D')
            {
                contador++;
                mapa[linha-1][coluna+1] = '*';
                afunda_destroyer(mapa,linha-1,coluna+1,rowMax,columnMax,contador);
            }
        }

        if(coluna != columnMax)
        {
            if(mapa[linha][coluna+1] == 'D')
            {
                contador++;
                mapa[linha][coluna+1] = '*';
                afunda_destroyer(mapa,linha,coluna+1,rowMax,columnMax,contador);
            }
        }

        if(linha != rowMax && coluna != columnMax)
        {
            if(mapa[linha+1][coluna+1] == 'D')
            {
                contador++;
                mapa[linha+1][coluna+1] = '*';
                afunda_destroyer(mapa,linha+1,coluna+1,rowMax,columnMax,contador);
            }
        }
    }
}
