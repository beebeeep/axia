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

/** 
 * sturcture representing file record in file index
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

#endif
