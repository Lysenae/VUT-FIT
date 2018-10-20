#Daniel Klimaj, xklima22@stud.fit.vutbr.cz
NAME=xklima22
RM=rm -f

all:
	pdflatex $(NAME)
	bibtex $(NAME)
	pdflatex $(NAME)
	pdflatex $(NAME)

clean:
	$(RM) $(NAME).aux
	$(RM) $(NAME).log
	$(RM) $(NAME).pdf
	$(RM) $(NAME).out
	$(RM) $(NAME).bbl
	$(RM) $(NAME).blg
