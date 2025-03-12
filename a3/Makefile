CC = gcc
CFLAGS = -Wall -ggdb -lc -static

SOURCES = $(wildcard sploit?.c)
TARGETS = $(patsubst %.c,%,$(SOURCES))

all: $(TARGETS)

sploit%: sploit%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TARGETS)
