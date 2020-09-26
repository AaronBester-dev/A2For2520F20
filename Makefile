CFLAGS = -std=c99 -Wall -pedantic 
CC = gcc

all: array.o test1 test2 test3

array.o: array.c 
	$(CC) $(CFLAGS) -c array.c -o array.o

test1.o: test1.c
	$(CC) $(CFLAGS) -c test1.c -o test1.o

test1: test1.o array.h
	$(CC) $(CFLAGS) test1.o array.o -o test1

test2.o: test2.c
	$(CC) $(CFLAGS) -c test2.c -o test2.o

test2: test2.o array.h
	$(CC) $(CFLAGS) test2.o array.o -o test2

test3.o: test3.c
	$(CC) $(CFLAGS) -c test3.c -o test3.o

test3: test3.o array.h
	$(CC) $(CFLAGS) test3.o array.o -o test3


clean:
	rm -i array.o test1.o test1