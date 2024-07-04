CC = gcc

CHILD = child.c
PARENT = parent.c

all: parent child

parent: $(PARENT)
	$(CC) $(CFLAGS) -o $@ $(PARENT)

child: $(CHILD)
	$(CC) $(CFLAGS) -o $@ $(CHILD)

run: parent
	./parent

clean:
	rm -f parent child

.PHONY: all run clean
