CC=gcc
CFLAGS=-Wall -Wextra -lm

all: generate sort

generate: main.c
	$(CC) $(CFLAGS) main.c -o generate

sort: soure_index.c
	$(CC) $(CFLAGS) soure_index.c -o sort

clean:
	rm -f generate sort
