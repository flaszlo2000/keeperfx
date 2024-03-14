#pragma once

#include <stdlib.h>

typedef struct DynamicArray {
    char ** content;

    size_t used_size;
    size_t allocated_size;
} DynamicArray;

/*
Creates a dynamic array
@param sizeToAllocate the *count* of elements we want to store initially
@returns an instance of DynamicArray
*/
DynamicArray createDynamicArray(size_t sizeToAllocate);

/*
Inserts a string into the given DynamicArray and if that's allocated size should be increased, it takes care of that too.
@param darray: the DynamicArray to expand with the given data (and resize if needed)
@param data: string to put into the DynamicArray
*/
void insertToDynamicArray(DynamicArray * darray, char * data);

/*
Frees the content of the given DynamicArray but not the array itself.
@param darray: the DynamicArray to free up it's content
*/
void freeDynamicArrayContent(DynamicArray * darray);