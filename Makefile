CFLAGS = -std=c99 -Wall -pedantic 
CC = gcc

all: array.o

array.o: array.c
	$(CC) $(CFLAGS) -c array.c -o array.o

clean:
	rm -i array.o