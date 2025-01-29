all: clean
	pdflatex a1_responses.tex
	zip -j a1.zip sploit*.c ../key* a1_responses.pdf

clean:
	rm -f a1.zip a1_responses.pdf a1_responses.aux a1_responses.log a1_responses.out

.PHONY: all clean
