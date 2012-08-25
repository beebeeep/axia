#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

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

ax_list *ax_list_init(void *entry, void *(*alloc_func)(void *), int (*free_func)(void *) )
{
	ax_list *list =  (ax_list *)malloc(sizeof(ax_list));
	if(list == NULL) return NULL; 

	/* allocate memory for list entry and copy data into it */
	ax_list_entry *e = (ax_list_entry *)malloc(sizeof(ax_list_entry));
	if(e == NULL) {
		free(list);
		return NULL;
	}

	e->data = alloc_func(entry);		//call allocator that allocates memory and copies data 
	if(e->data == NULL) {
		free(list); free(e);
		return NULL;
	}

	e->next = NULL;
	e->prev = NULL;
	list->head = e;
	list->tail = e;
	list->alloc_func = alloc_func;
	list->free_func = free_func; 
	list->size = 1;

	return list;
}

ax_list_entry * ax_list_append(ax_list *list, void *entry)
{
	/* allocate memory for list entry and list data */
	ax_list_entry *e = (ax_list_entry *)malloc(sizeof(ax_list_entry));
	if(e == NULL) return NULL; 
	e->data = list->alloc_func(entry);
	if(e->data == NULL) {
		free(e); 
		return NULL;
	}
	
	e->next = NULL; 
	e->prev = list->tail;

	/* set pointer to created entry and shift list tail */
	list->tail->next = e; 
	list->tail = e; 
	list->size++;

	return e;
}

int ax_list_remove(ax_list *list, ax_list_entry *entry)
{
	int ret; 

	if(list == NULL || entry == NULL) return 1;

	if(entry->prev == NULL) {
		// head of list
		list->head = entry->next;
		list->head->prev = NULL;
	} else if(entry->next == NULL) {
		// tail of list
		list->tail = entry->prev;
		list->tail->next = NULL;
	} else {
		//middle of list
		entry->next->prev = entry->prev;
		entry->prev->next = entry->next;
	}
	
	list->size--;
	ret = list->free_func(entry->data);
	free(entry);
	return ret;
}

int ax_list_free(ax_list *list)
{
	if(list == NULL) return 1;

	ax_list_entry *e = list->head, *t;
	int ret; 
	
	while(e != NULL) {
		t = e->next;
		if( (ret = list->free_func(e->data)) != 0) return 2;
		free(e);
		e = t;
	}
	return 0;
}

void ax_list_dump(ax_list *list) 
{
	ax_list_entry *e = list->head;
	size_t c = 0;

	while(e != NULL) {
		printf("\nElem #%d 0x%x, prev = 0x%x, next = 0x%x\n", c++, e, e->prev, e->next);
		e = e->next;
	}
}
