/**
@file	misc.h
@author	Migalin Danila <miga@migalin.net>
@brief	Miscellaneous functions
*/

#ifndef MISC_H
#define MISC_H


/** 
 * linked list entry
 */
typedef struct t_ax_list_entry {
	void *entry;			
	struct t_ax_list_entry *next;
} ax_list_entry;

/**
 * linked list
*/
typedef struct t_ax_list {
	ax_list_entry *head;
	ax_list_entry *tail;
	size_t size;
} ax_list;

/**
Simple logger function

@param	str	log string
*/
void ax_log(char *str); 

/**
Allocates sufficient amount of memory needed to store
path to file. Copypasted from APUE

@param sizep pointer to store amount of allocated memory
@return allocated buffer
*/
char *ax_path_alloc(int *sizep);


/** 
 * Creates new linked list
*/
ax_list *ax_list_init(void);

/** 
Appends entry to linked list
@param list pointer to list structure
@param entry pointer to entry buffer
@param size size of entry
@return pointer to created list entry
*/
ax_list_entry * ax_list_append(ax_list *list, void *entry, size_t size);

#endif

