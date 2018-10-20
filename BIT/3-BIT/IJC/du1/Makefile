GCC   = gcc
FLAGS = -O2 -std=c99 -Wall -pedantic
LIBS  = -lm
PARAM = -DUSE_INLINE

all: steg-decode primes

error.o: error.c
	$(GCC) error.c -c $(FLAGS)

eratosthenes.o: eratosthenes.c error.o
	$(GCC) eratosthenes.c -c $(FLAGS)

ppm.o: error.o ppm.c
	$(GCC) ppm.c -c $(FLAGS)

primes.o: primes.c eratosthenes.o
	$(GCC) primes.c -c $(FLAGS)

steg-decode.o: steg-decode.c eratosthenes.o
	$(GCC) steg-decode.c -c $(FLAGS)

steg-decode: error.o eratosthenes.o ppm.o steg-decode.o
	$(GCC) error.o eratosthenes.o ppm.o steg-decode.o $(FLAGS) -o steg-decode $(LIBS)

primes: error.o eratosthenes.o primes.o
	$(GCC) error.o eratosthenes.o primes.o $(FLAGS) -o primes $(LIBS)
	$(GCC) $(PARAM) error.o eratosthenes.o primes.o $(FLAGS) -o primes-inline $(LIBS)

clean:
	rm -f *.o
	rm -f primes
	rm -f primes-inline
	rm -f steg-decode
	rm -f *.zip

pack: clean
	zip xklima22.zip *.c *.h Makefile