# Parallel programming
# Assignment 4, exercise 1
# Group 165
# usage for sendrecv: mpiexec --hostfile hosts ./ex1-sendrecv
# usage for bcastreduce: mpiexec --hostfile hosts ./ex1-bcastreduce

CC = mpicc
CFLAGS= --std=c99


complete : ex1-sendrecv ex1-bcastreduce
	echo "usage for sendrecv: mpiexec --hostfile hosts ./ex1-sendrecv /n usage for bcastreduce: mpiexec --hostfile hosts ./ex1-bcastreduce"

ex1-sendrecv : ex1-sendrecv.c
	$(CC) $(CFLAGS) ex1-sendrecv.c -o ex1-sendrecv

ex1-bcastreduce : ex1-bcastreduce.c
	$(CC) $(CFLAGS) ex1-bcastreduce.c -o ex1-bcastreduce

clean :
	rm ex1-sendrecv ex1-bcastreduce

