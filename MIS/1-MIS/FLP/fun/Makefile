# Projekt: FLP #1 - plg-2-nka
# Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
# Datum:   2017-02-27
# Soubor:  Makefile

FILE = plg-2-nka
PACK = flp-fun-xklima22.zip

all:
	ghc --make $(FILE)

clean:
	rm -f $(FILE)
	rm -f $(PACK)
	find . -name "*.o" -type f -delete
	find . -name "*.hi" -type f -delete
	find . -name ".fuse_*" -type f -delete

pack: clean
	zip -r $(PACK) .

test: all
	bash test.sh
