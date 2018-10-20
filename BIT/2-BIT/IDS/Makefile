#!/bin/sh
# LaTeX makefile
FILE=documentation
all:$(FILE)

.PHONY:clean
clean:
	\rm *.aux *.blg *.out *.bbl *.log *.ps *.dvi
$(FILE):$(FILE).tex
	latex $(FILE).tex
	latex $(FILE).tex
	dvips $(FILE).dvi
	ps2pdf -sPAPERSIZE=a4 $(FILE).ps $(FILE).pdf
