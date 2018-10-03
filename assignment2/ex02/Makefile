# Parallel programming
# Assignment 2, exercise 1
# Group 55
# usage: ex1 T NT
# build an executable named ex1 from ex1.cpp
CC = gcc
CFLAGS=-Wall -pthread

ex2 : ex2.o 
	$(CC) $(CFLAGS) -o ex2 ex2.o -lm

ex2.o : ex2.c
	$(CC) $(CFLAGS) -c ex2.c 

clean :
	rm *.o ex2 