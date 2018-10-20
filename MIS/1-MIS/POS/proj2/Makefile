# Projekt: POS #2 - Shell
# Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
# Datum:   2017-04-09
# Soubor:  Makefile

CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LIBS=-lpthread
PROG=shell
FILES=utils.o monitor.o str_vector.o process.o shell.o
CC=gcc

all: $(PROG)

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

monitor.o: monitor.c
	$(CC) $(CFLAGS) -c monitor.c

str_vector.o: str_vector.c
	$(CC) $(CFLAGS) -c str_vector.c

process.o: process.c
	$(CC) $(CFLAGS) -c process.c

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

.PHONY: clean

$(PROG): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(PROG) $(LIBS)

run:
	./$(PROG)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	rm -f $(PROG).out
	rm -f .fuse*
