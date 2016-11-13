CC=gcc
CFLAGS+= -g -Wall -std=c99
LDFLAGS+= 
LIBS= -l cgraph

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

test: grafo.o test.o minunit.h
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

all: grafo.o test.o minunit.h
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
