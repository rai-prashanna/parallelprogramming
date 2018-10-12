# Parallel programming
# Assignment 3, exercise 3
# Group 105
# usage: ex3-standard N T (N=size, T=number of threads)
# usage: ex3-tiled N T    (N=size, T=number of threads)
# build an executable named ex3-standard from ex3-standard.c
# build an executable named ex3-tiled from ex3-tiled.c

CC = gcc
CFLAGS=-Wall -fopenmp

ex3-standard : ex3-standard.o
	$(CC) $(CFLAGS) -o ex3-standard ex3-standard.o -lm

ex3-standard.o : ex3-standard.c
	$(CC) $(CFLAGS) -c ex3-standard.c

clean :
	rm *.o ex3-standard


ex3-tiled : ex3-tiled.o
	$(CC) $(CFLAGS) -o ex3-tiled ex3-tiled.o -lm

ex3-tiled.o : ex3-tiled.c
	$(CC) $(CFLAGS) -c ex3-tiled.c

clean :
	rm *.o ex3-tiled
