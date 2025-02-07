CC = gcc
CFLAGS = -Wall -ggdb
OUTDIR = test

SOURCES = $(wildcard sploit?.c)
TARGETS = $(patsubst %.c,$(OUTDIR)/%,$(SOURCES))

all: $(OUTDIR) $(TARGETS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/sploit%: sploit%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OUTDIR)
