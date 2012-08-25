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
	void *data;			
	struct t_ax_list_entry *next;
	struct t_ax_list_entry *prev;
} ax_list_entry;

/**
 * linked list
*/
typedef struct t_ax_list {
	ax_list_entry *head;			/**< head of list */	
	ax_list_entry *tail;			/**< tail of list */
	void *(*alloc_func)(void *);		/**< pointer to function that allocates and copies list entry */
	int (*free_func)(void *);		/**< pointer to function that frees allocated memory for this entry */
	size_t size;				/**< size of list */
} ax_list;

/**
 * Simple logger function
 * @param	str	log string
*/
void ax_log(char *str); 

/**
 * Allocates sufficient amount of memory needed to store
 * path to file. Copypasted from APUE
 * @param sizep pointer to store amount of allocated memory
 * @return allocated buffer
*/
char *ax_path_alloc(int *sizep);


/** 
 * Creates new linked list
 *
 * @param entry first list element
 * @param size size of element
 * @param alloc_func pointer to allocator
 * @param free_func pointer to deallocator
*/
ax_list *ax_list_init(void *entry, void *(*alloc_func)(void *), int (*free_func)(void *) );

/** 
 * Appends entry to linked list
 * @param list pointer to list structure
 * @param entry pointer to entry buffer
 * @return pointer to created list entry
*/
ax_list_entry *ax_list_append(ax_list *list, void *entry);

/**
 * this function removes entry from list
 * 
 * @param entry entry to remove
 * @return integer, 0 - success
*/
int ax_list_remove(ax_list *list, ax_list_entry *entry); 

/**
 * frees list and all its elements
 *
 * @param list list to free
 * @return integer, 0 - success
*/
int ax_list_free(ax_list *list);

void ax_list_dump(ax_list *list);

#endif

