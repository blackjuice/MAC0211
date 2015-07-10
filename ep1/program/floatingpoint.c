#include <stdio.h>

void int2str (int n) {
    int i, j;
    int prime;
    for (i = 2; i < n; i++) {
	for (j = 2; j < i; j++) {
	    printf("j = %d\n", j);
	    if (!(i % j)) {
		prime = 0;
		break;
	    }
	    prime = 1;
	}
	if (prime) printf("prime = %d\n", i);
    }
}

int main () {
    int n = 5;
    int2str (n);
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
