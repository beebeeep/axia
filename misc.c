#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>

#include "misc.h"

#define SUSV3 200112L
#define PATH_MAX_GUESS 1024

#ifdef PATH_MAX
static size_t pathmax = PATH_MAX;
#else 
static size_t pathmax = 0;
#endif

static long posix_version = 0;


void ax_log(char *str) 
{
	time_t t;
	char t_str[30];
	time(&t);
	strftime(t_str, 30, "[%d.%m.%Y %X] ", localtime(&t));

	fprintf(stderr, t_str);
	fprintf(stderr, str);
}

char *ax_path_alloc(int *sizep) 
{
	char *path;
	int size;

	if(posix_version == 0) 
		posix_version = sysconf(_SC_VERSION);

	if(pathmax == 0) {
		errno = 0;
		
		if((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
			if(errno == 0) 
				pathmax = PATH_MAX_GUESS;
			else
				ax_log("Error calling pathconf()!\n"); 
				return NULL;
		} else {
			pathmax++;
		}
	}

	if(posix_version < SUSV3) 
		size = pathmax + 1;
	else
		size = pathmax;

	if((path = malloc(size)) == NULL) {
		ax_log("Cannot allocate memory for path string");
		return NULL;
	} 

	if(sizep != NULL) 
		*sizep = size;

	return path;
}
