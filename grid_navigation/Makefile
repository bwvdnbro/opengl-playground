CFLAGS = -g -fsanitize=address -fno-omit-frame-pointer

all: main

main: main.o grid.o cell.o
	gcc $(CFLAGS) -o main main.o grid.o cell.o

main.o: main.c grid.h
	gcc $(CFLAGS) -c main.c

grid.o: grid.c grid.h cell.h
	gcc $(CFLAGS) -c grid.c

cell.o: cell.c
	gcc $(CFLAGS) -c cell.c
