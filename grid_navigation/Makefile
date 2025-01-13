CFLAGS = -g -fsanitize=address -fno-omit-frame-pointer

all: main

main: main.o grid.o cell.o network_graph.o
	gcc $(CFLAGS) -o main main.o grid.o cell.o network_graph.o

main.o: main.c grid.h network_graph.h
	gcc $(CFLAGS) -c main.c

grid.o: grid.c grid.h cell.h
	gcc $(CFLAGS) -c grid.c

cell.o: cell.c
	gcc $(CFLAGS) -c cell.c

network_graph.o: network_graph.c network_graph.h grid.h cell.h
	gcc $(CFLAGS) -c network_graph.c
