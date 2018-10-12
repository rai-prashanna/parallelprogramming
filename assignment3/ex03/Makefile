# Parallel programming
# Assignment 3, exercise 3
# Group 105
# usage: ex3-standard N
# build an executable named ex3-standard from ex3-standard.c

CC = gcc
CFLAGS=-Wall -fopenmp

ex02 : ex3-standard.o
	$(CC) $(CFLAGS) -o ex3-standard ex3-standard.o -lm

ex02.o : ex3-standard.c
	$(CC) $(CFLAGS) -c ex3-standard.c

clean :
	rm *.o ex3-standard
