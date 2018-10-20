#Daniel Klimaj, xklima22@stud.fit.vutbr.cz
NAME=dokumentace

all:
	vlna -l -m -n $(NAME).tex
	pdflatex $(NAME).tex
	bibtex $(NAME).aux
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

clean:
	$(RM) $(NAME).aux $(NAME).log
	$(RM) $(NAME).pdf $(NAME).ps
	$(RM) $(NAME).dvi
	$(RM) $(NAME).bbl
	$(RM) $(NAME).blg
