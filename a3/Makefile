CC = gcc
CFLAGS = -Wall -ggdb

SOURCES = $(wildcard sploit?.c)
TARGETS = $(patsubst %.c,%,$(SOURCES))

all: $(TARGETS)

sploit1: sploit1.c
	$(CC) $(CFLAGS) $< -o $@

sploit2: sploit2.c
	$(CC) $(CFLAGS) -static $< -o $@

sploit3: sploit3.c
	$(CC) $(CFLAGS) -m32 $< -o $@

sploitA: sploitA.c
	$(CC) $(CFLAGS) -static -Wno-unused-variable $< -o $@

sploitB: sploitB.c
	$(CC) $(CFLAGS) -static $< -o $@ -luring

clean:
	rm -rf $(TARGETS) j76xiao/ a3.zip

submit: clean
	mkdir j76xiao
	cp sploit[1-4].c Makefile j76xiao/
	zip -r - j76xiao/ | curl --data-binary @- ugster72c.student.cs.uwaterloo.ca:9000/submit
	cp sploit[AB].c j76xiao/
	zip -rj a3.zip j76xiao/

.PHONY: all clean submit
