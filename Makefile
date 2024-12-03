CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -fsanitize=address
OBJS = main.c avl.c set.c
DEPS = avl.h set.h

all: $(DEPS)
	$(CC) $(CFLAGS) $(OBJS) -o programa
run:
	./programa
clean:
	rm *.o programa