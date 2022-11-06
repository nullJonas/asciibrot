CFLAGS = -std=c99
LDFLAGS = -lm -lncurses

all: asciibrot

asciibrot: code/main.c
	gcc $(CFLAGS) code/main.c -o asciibrot $(LDFLAGS)

clean:
	rm asciibrot