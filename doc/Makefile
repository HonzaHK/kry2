proj=doc
all:
	latex $(proj).tex
	latex $(proj).tex
	dvips -t a4 $(proj).dvi
	ps2pdf $(proj).ps

clean: rm $(proj).log $(proj).ps $(proj).dvi $(proj).aux
