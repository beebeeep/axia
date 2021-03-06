/**
@file	indexer.h
@author	Migalin Danila <miga@migalin.net>
@brief	Indexer functions

@details
This file contents indexer that scans specified 
directory indexing all files, its attributes and content type
*/

#ifndef INDEXER_H
#define INDEXER_H

#include "misc.h"

/**
 * structure representing file entry
*/
typedef struct {
	char		*cname;		/**< canonicalized file name */
	struct stat	st;		/**< stat structure */
} ax_file_entry;

/** 
 * stucture representing file record in file index
*/
typedef struct {
	char 	*cname;		/**< canonicalized file name */
	char 	*size;		/**< size of file */
	time_t	ctime;		/**< creation timestamp */
	time_t	atime;		/**< access timestamp */
	time_t	mtime;		/**< modify timestamp */
	/* TODO other attributes */
} ax_file_record;

int ax_build_index(char *, char *);

/**
 * this function allocates memory for file entry and copies data from buffer 
 * to newly allocated structure
 * 
 * @param entry pointer to buffer with entry to copy
 * @return pointer to allocated entry
 */
void *ax_file_entry_allocator(void *entry);


/** 
 * this function frees memory allocated by ax_file_entry_allocator()
 * 
 * @param entry entry to free
 * @return	integer value, 0 - success
*/
int ax_file_entry_free(void *entry);
 
/**
 * this function scans directory stored in list entry and
 * appends its contents into files list
 * 
 * @param files list with file entries
 * @param dir dir entry to be scanned
 * @return n/a
 */
void ax_append_dir_content(ax_list *files, ax_list_entry *dir);

#endif
