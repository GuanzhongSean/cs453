CC = gcc
CFLAGS = -Wall -ggdb -static

SOURCES = $(wildcard sploit?.c)
TARGETS = $(patsubst %.c,%,$(SOURCES))

URL := ugster72c.student.cs.uwaterloo.ca:9000/submit

all: $(TARGETS)

sploit3: sploit3.c
	$(CC) $(CFLAGS) -m32 $< -o $@

sploitB: sploitB.c
	$(CC) $(CFLAGS) $< -o $@ -luring

clean:
	rm -rf $(TARGETS) j76xiao/ a3.zip

zip: clean
	mkdir j76xiao
	pdflatex write-up.tex
	cp sploit[1-4].c Makefile write-up.pdf j76xiao/
	zip -rj91 a3.zip j76xiao/

submit: zip
	zip -qr - j76xiao/ | curl --data-binary @- $(URL)

.PHONY: all clean zip submit
