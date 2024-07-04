CC = g++
CFLAGS = -Wall -g

SRCS = Ring.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = Fives

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	@rm -f $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(EXEC) $(OBJS)
