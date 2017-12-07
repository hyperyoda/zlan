CC = gcc
CFLAGS = -g -O

zlan : zlan.o
	${CC} ${CFLAGS} -o zlan zlan.o

zlan.o : zlan.c
	${CC} ${CFLAGS} -c zlan.c

clean:
	-rm -f *.o zlan