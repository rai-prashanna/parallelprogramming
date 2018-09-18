# build an executable named myprog from myprog.c
all: myprog.c 
 	g++ -std=c++11 -Wall -pthread -o myprog myprog.c

clean:
	$(RM) myprog
