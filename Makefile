PROG=axia
CC=gcc
LD=gcc -o
CFLAGS=-Wall -g -ldb
FUSEFLAGS=`pkg-config fuse --cflags --libs`

all: $(PROG).c indexer.o misc.o
	$(CC) $(CFLAGS) $(FUSEFLAGS) -c $(PROG).c
	$(LD) $(CFLAGS) $(FUSEFLAGS) -o $(PROG) $(PROG).o indexer.o misc.o 

clean: 
	rm -f *.o $(PROG)

