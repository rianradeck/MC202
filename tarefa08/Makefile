CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: listas_legais

listas_legais: listas_legais.c
	gcc $(CFLAGS) listas_legais.c -o listas_legais $(LDFLAGS)

clean:
	rm -f listas_legais
	rm -f *.o
