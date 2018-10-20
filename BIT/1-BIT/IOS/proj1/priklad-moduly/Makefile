OBJS=recfun.o print_recursive.o recursive.o timeout.o
PROGNAME=recfun

CFLAGS=-std=gnu99 -O0 -g
CC=gcc

.PHONY: all dep clean

$(PROGNAME): $(OBJS)

dep:
	$(CC) -MM *.c >Makefile.dep

clean:
	-rm $(PROGNAME) $(OBJS)

-include Makefile.dep

