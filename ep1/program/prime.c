#include <stdio.h>

void int2str (int n) {
    printf ("%d\n", n);
}

int main () {
    int i, j;
    int prime;

    for (i = 2; i < 1000; i++) {
        prime = 1;
        for (j = 2; j < i; j++) {
            //printf("i = %d | j = %d\n", i, j);
            if ((i % j) == 0) {
                prime = 0;
                break;
            }
            //prime = 1;
        }
        if (prime) int2str(i);
        //if (prime == 1) printf("pŕime = %d\n", i);
    }
    return 0;
}

/*
//start loop1
i = 2;
loop:
    if (!(i < 1000))
        go to end1;
    //start loop2
    j = 2;
    loop2:
	if (!(j < i))
	    go to end2;

	if ((i % j) == 0)
	    prime = false;
	    break;
	else prime = true;

	j++;
    	go to loop2;

    end2; //end loop2	
    if (prime) printf ("%d\n", i)

    i++;
    go to loop;
end1;
//end loop1
*/
