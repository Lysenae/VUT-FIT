# Projekt: FLP #2 - Turingov stroj
# Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
# Datum:   2017-04-28
# Subor:   Makefile

FILE = flp17-log
PACK = flp-log-xklima22.zip

all:
	swipl -q --goal=main -o $(FILE) -c $(FILE).pl

clean:
	rm -f $(FILE)
	rm -f $(PACK)
	rm -f .fuse*

pack: clean
	zip -r $(PACK) .

test: all
	bash test.sh
