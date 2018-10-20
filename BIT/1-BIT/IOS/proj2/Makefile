# Soubor:  Makefile
# Datum:   2014/04/27
# Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
# Popis:   makefile pre projekt c. 2 do IOS

PRE=-std=gnu99 -Wall -Wextra -Werror -pedantic
END=-lpthread
PROG=rivercrossing
MAIN=main

all:	$(MAIN)

.PHONY: clean

$(MAIN): $(MAIN).c
	gcc $(PRE) $(MAIN).c -o $(PROG) $(END)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	rm -f $(PROG).out
