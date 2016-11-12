CC=gcc

all : grafo.c grafo.h
	$(CC) -g grafo.c test.c -o test -std=c99 -l cgraph

test: grafo.h grafo.c test.c minunit.h
	$(CC) -g grafo.c test.c -o test -std=c99 -l cgraph
	./test 12 sample
