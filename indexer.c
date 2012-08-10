#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <db.h>

#include "misc.h"



DB* ax_prepare_db(char *db_name) {
	DB *dbp;

	u_int32_t flags;
	int ret;

	ret = db_create(&dbp, NULL, 0);
	if(ret != 0) {
		ax_log("Cannot create DB handler!\n");
		return NULL;
	}

	flags = DB_CREATE;

	ret = dbp->open(dbp, NULL, db_name, NULL, DB_HASH, flags, 0);
	if (ret != 0) {
		ax_log("Cannot create DB!\n");
		return NULL;
	}

	if(dbp != NULL) {
		dbp->close(dbp, 0);
	}
	
	return dbp;
}

int ax_build_index(char *path, char *db_name) 
{
	ax_log("Starting indexing files...\n");

	DIR *dir; 
	struct dirent *entry;
	struct stat statbuf;
	int ret;

	char *fullpath, *ptr;
	int len; 

	fullpath = ax_path_alloc(&len);
	realpath(path, fullpath);
	
	dir = opendir(fullpath);
	if(lstat(fullpath, &statbuf)) {
		ax_log("Cannot stat start path!\n");
		return -1;
	}
	if(!S_ISDIR(statbuf.st_mode)) {
		ax_log("Start path is not a directory!\n");
		return -2;
	}

	ptr = fullpath + strlen(fullpath);
	*ptr++ = '/';						//add slash at the end
	*ptr = '\0';


	printf("searching in %s\n", fullpath);	
	while( (entry = readdir(dir)) != NULL) {
		if(entry->d_name[0] == '.') continue;		//skip hidden files and current and parent dir
		
		printf("%s\n", entry->d_name);
	}
	return 0;
}
