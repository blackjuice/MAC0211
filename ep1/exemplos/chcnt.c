#include <stdio.h>

int main()
{
    unsigned long nc;
    nc = 0;
    while (getchar() != EOF)
        nc++;
    printf("%lu\n", nc);
    return 0;
}
