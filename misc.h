/**
@file	misc.h
@author	Migalin Danila <miga@migalin.net>
@brief	Miscellaneous functions
*/

#ifndef MISC_H
#define MISC_H

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

#endif

