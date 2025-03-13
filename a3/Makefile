CC = gcc
CFLAGS = -Wall -ggdb -static

SOURCES = $(wildcard sploit?.c)
TARGETS = $(patsubst %.c,%,$(SOURCES))

all: $(TARGETS)

sploit3: sploit3.c
	$(CC) $(CFLAGS) -m32 $< -o $@

sploitB: sploitB.c
	$(CC) $(CFLAGS) $< -o $@ -luring

clean:
	rm -rf $(TARGETS) j76xiao/ a3.zip

submit: clean
	mkdir j76xiao
	cp sploit[1-4].c Makefile j76xiao/
	zip -qr - j76xiao/ | curl --data-binary @- ugster72c.student.cs.uwaterloo.ca:9000/submit

zip:
	zip -r91 a3.zip sploit?.c Makefile write-up.pdf

.PHONY: all clean submit
