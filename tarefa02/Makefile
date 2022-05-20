CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all 

all: professor_carlos

professor_carlos: professor_carlos_main.c
	gcc $(CFLAGS) -c professor_carlos_funcoes.c $(LDFLAGS)
	gcc $(CFLAGS) -c professor_carlos_main.c $(LDFLAGS)
	gcc $(CFLAGS) -o professor_carlos professor_carlos_main.o professor_carlos_funcoes.o $(LDFLAGS)

