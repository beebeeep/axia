#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <db.h>

#include "misc.h"
#include "indexer.h"


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
	struct dirent *file_dirent;
	struct stat statbuf;
	int ret;

	char *fullpath, *ptr, *filename;
	int len; 

	ax_list *files;
	ax_list_entry *current_file;
	
	ax_file_entry d_entry;

	fullpath = ax_path_alloc(&len);
	filename = ax_path_alloc(&len);

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

	d_entry.cname = fullpath;
	d_entry.st = statbuf; 

	files = ax_list_init(&d_entry, sizeof(d_entry));
	current_file = files->head; 

	while(current_file != NULL) {
		ax_file_entry *e = (ax_file_entry *)current_file->data;
		/* if current file entry is directory, scan it and append to files list 
		 * content of this directory 
		 */
		if(S_ISDIR(e->st.st_mode)) {
		       	ax_append_dir_content(files, current_file);
		}
		else {
			/* TODO create file record and save it in DB */
		}

		/* scan for next file in list */
		current_file = current_file->next;
	}

	return 0;
}

void ax_append_dir_content(ax_list *files, ax_list_entry *dir) 
{
	ax_file_entry *e = (ax_file_entry *)dir->data;
	DIR *directory;
	struct dirent *file;
	if(!S_ISDIR(e->st.st_mode)) return;

	directory = opendir(e->cname);

	while(file = readdir(directory) != NULL) {
		//scan dir
	}
}
