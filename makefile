CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0`

all: clean 1 25 PSdevise

1: division1.c
	${CC} ${CFLAGS} -o 1 division1.c ${LIBS} -w

25: division25.c
	${CC} ${CFLAGS} -o 25 division25.c ${LIBS} -w

PSdevise: PSdevise.c
	${CC} -o PSdevise PSdevise.c -lvlc

.PHONY: clean

clean:
	-rm -f *.o 1 25 PSdevise