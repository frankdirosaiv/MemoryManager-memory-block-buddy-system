# makefile

all: memtest

ackerman.o: ackerman.c 
	gcc -c -std=c99 -g ackerman.c

my_allocator.o : my_allocator.c
	gcc -c -std=c99 -g my_allocator.c

memtest.o : memtest.c
	gcc -c -std=c99 -g memtest.c

memtest: memtest.o ackerman.o my_allocator.o
	gcc -o memtest memtest.o ackerman.o my_allocator.o

