# Soubor:  Makefile
# Datum:   2013/10/21
# Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
# Popis:   makefile pre proj1.c do predmetu IZP

PRE=-std=c99 -Wall -Wextra -pedantic
PROG=proj1

all:	$(PROG)

.PHONY: clean

$(PROG): $(PROG).c
	gcc $(PRE) $(PROG).c -o $(PROG)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
