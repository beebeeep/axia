#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>


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

	char *fullpath, *ptr;
	int len; 

	ax_list *files;
	ax_list_entry *current_file;
	
	ax_file_entry d_entry;

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

	d_entry.cname = fullpath;
	d_entry.st = statbuf; 

	files = ax_list_init(&d_entry, ax_file_entry_allocator, ax_file_entry_free);
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
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	fprintf(stderr, "Resource usage:\n\treads=%ld\n\twrites=%ld\n\tmaxrss=%ld\n", usage.ru_inblock, usage.ru_oublock, usage.ru_maxrss);


	return 0;
}

void ax_append_dir_content(ax_list *files, ax_list_entry *dir) 
{
	ax_file_entry *e = (ax_file_entry *)dir->data;
	ax_file_entry new_entry, *old_entry_ptr;
	DIR *directory;
	struct dirent *entry;
	struct stat statbuf;
	char *filename;
        char *ptr;
	int len;
	size_t path_len = strlen(e->cname);

	if(!S_ISDIR(e->st.st_mode)) return;

	filename = ax_path_alloc(&len);
	
	directory = opendir(e->cname);
	if(directory == NULL) {
		fprintf(stderr, "Cannot open dir %s: ", e->cname);
		perror("");
		return;
	}

	printf("scanning dir %s\n", e->cname);
	while((entry = readdir(directory)) != NULL) {
		if(entry->d_name[0] == '.') continue;

		sprintf(filename, "%s%s", e->cname, entry->d_name);
		printf("\tfound file '%s'\n", filename);

		if(stat(filename, &statbuf)) {
			fprintf(stderr, "Cannot stat %s: ", filename);
			perror("");
			continue;
		}

		/* looks like such byte-level operations
		 * shouldn't break anything with 
		 * 8-bit and UTF-8 locales
		 */
		ptr = filename + strlen(filename);
	       	*ptr++	= '/'; 
		*ptr = '\0';
		
		new_entry.st = statbuf;
		new_entry.cname = filename;		

		ax_list_append(files, &new_entry);
	}
	free(filename);

	ax_list_remove(files, dir);

	if(closedir(directory)) {
		fprintf(stderr, "Cannot close %s: ", e->cname);
		perror("");
	}
}


int ax_file_entry_free(void *entry)
{
	if(entry == NULL) return 1;
	ax_file_entry *e = (ax_file_entry *)entry;

	free(e->cname);
	free(e);
	return 0;
}

void *ax_file_entry_allocator(void *entry)
{
	if(entry == NULL) return NULL;
	ax_file_entry *e = (ax_file_entry *)entry;
	ax_file_entry *r = (ax_file_entry *)malloc(sizeof(ax_file_entry));

	if(r == NULL) return NULL;

	r->cname = (char *)malloc(strlen(e->cname)+1);

	if(r->cname == NULL) {
		free(r);
		return NULL;
	}

	strncpy(r->cname, e->cname, strlen(e->cname)+1);
	r->st = e->st;
	
	return r;
}

