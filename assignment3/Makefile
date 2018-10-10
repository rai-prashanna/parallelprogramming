# Parallel programming
# Assignment 3, exercise 1
# Group 105
# usage: ex2 number_of_threads
# build an executable named ex01 from ex01.c

CC = gcc
CFLAGS=-Wall -pthread -fopenmp

ex01 : ex01.o
	$(CC) $(CFLAGS) -o ex01 ex01.o -lm

ex01.o : ex01.c
	$(CC) $(CFLAGS) -c ex01.c

clean :
	rm *.o ex01

