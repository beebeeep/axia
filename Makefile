PROG=axia
INDEXER=indexer
CC=gcc
LD=gcc -o
CFLAGS=-Wall -g -ldb
FUSEFLAGS=`pkg-config fuse --cflags --libs`

all: $(PROG).c $(INDEXER).o
	$(CC) $(CFLAGS) $(FUSEFLAGS) -c $(PROG).c
	$(LD) $(CFLAGS) $(FUSEFLAGS) -o $(PROG) $(PROG).o $(INDEXER).o 

clean: 
	rm -f *.o $(PROG)

