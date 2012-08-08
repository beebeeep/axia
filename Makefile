PROG=axia
CC=gcc
LD=gcc -o
CFLAGS=-Wall -g 
FUSEFLAGS=`pkg-config fuse --cflags --libs`

all: $(PROG).c
	$(CC) $(PROG).c $(CFLAGS) $(FUSEFLAGS) -o $(PROG)

clean: 
	rm -f *.o $(PROG)

