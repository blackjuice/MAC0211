CFLAGS=-Wall -g  -I -g -O2


all : teste7 ep3_7648325_8124329

teste7 : teste7.o xwc.o
	${CC} ${CFLAGS} -o teste7 teste7.o xwc.o  -lm -lXpm -lX11

ep3_7648325_8124329 : ep3_7648325_8124329.o xwc.o
	${CC} ${CFLAGS} -o ep3_7648325_8124329 ep3_7648325_8124329.o xwc.o -lm -lXpm -lX11

xwc.o : xwc.c


clean: 
	rm -f *.o teste7 ep3_7648325_8124329 *~ Nose.xpm Tutti.xpm caca

