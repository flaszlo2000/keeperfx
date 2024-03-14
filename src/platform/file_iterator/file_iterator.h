#pragma once

#include <stdio.h>
#include "utils/dynamic_array.h"

typedef struct FileIterator
{
    DynamicArray files;

    char * path;
    char * filespec; 

    size_t current;
    size_t count;
} FileIterator;

/*
Initializes FileItarator and fills it up based on the given parameters
@param path: path to directory
@param filespec: file extension, WARNING: not regex!
@param fullpath: result will be put there, you must take care of it's size
@returns 0 if finding was not successful, n > 0 that represents the amount of found files (FileIterator's count will be set based on this)
*/
short findfirst(const char * path, const char * filespec, char * fullpath);


/*
On *the* ititialized iterator, gives back the next filename
@param fullpath: result will be put there, you must take care of it's size
@returns 0 if finding was not successful, 1 if it was.
*/
short findnext(char * fullpath);

/*
Must call this after findfirst
*/
void findclose();