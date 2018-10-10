# Parallel programming
# Assignment 3, exercise 2
# Group 105
# usage: ex2 ArraySize TimeSteps
# build an executable named ex02 from ex02.c

CC = gcc
CFLAGS=-Wall -pthread -fopenmp

ex02 : ex02.o
	$(CC) $(CFLAGS) -o ex02 ex02.o -lm

ex02.o : ex02.c
	$(CC) $(CFLAGS) -c ex02.c

clean :
	rm *.o ex02

