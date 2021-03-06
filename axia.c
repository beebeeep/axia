#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "indexer.h"

const char *DB_NAME = "./index.db";


int main(int argc, char *argv[])
{
	if(argc > 1) { 
		ax_build_index(argv[1], DB_NAME);
	} else {
		ax_build_index("..", DB_NAME);
	}
	//return fuse_main(argc, argv, &hello_oper, NULL);
}
