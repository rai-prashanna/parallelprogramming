# Parallel programming
# Assignment 2, exercise 2
# Group 55
# build an executable named task2
CC = gcc
CFLAGS=-Wall -pthread

task2 : task2.o 
	$(CC) $(CFLAGS) -o task2 task2.o -lm

task2.o : task2.c
	$(CC) $(CFLAGS) -c task2.c 

clean :
	rm *.o task2 