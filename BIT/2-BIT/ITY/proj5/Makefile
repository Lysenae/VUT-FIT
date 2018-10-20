# ITY Projekt 5
# Autor: Tomáš Žigárdi, xzigar00@stud.fit.vutbr.cz
NAME=proj5

all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps


clean:
	$(RM) $(NAME).aux $(NAME).log
	$(RM) $(NAME).pdf $(NAME).ps
	$(RM) $(NAME).dvi
