#include <stdio.h>
#define true 1
#define false 0

int main () {
    int i, j;
    int prime = true;
    for (i = 0; i < 1000; i++) {
	for (j = 0; j < i / 2; j++) {
	    if (!(i % j)) {
		prime = false;
		break;
	    }
	    else prime = true;
	}
	if (prime) printf("%d, ", i);
    }
    printf("\n");
    return 0;
}
