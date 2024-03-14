#include "dynamic_array.h"
#include <string.h>

DynamicArray createDynamicArray(size_t size_to_allocate) {
    DynamicArray result;

    result.content = malloc(sizeof(char*) * size_to_allocate); // TODO error checking
    result.allocated_size = size_to_allocate;
    result.used_size = 0;

    return result;
}

void resizeDynamicArray(DynamicArray * darray) {
    size_t new_size = darray->allocated_size * 2;

    darray->content = (char**)realloc(darray->content, sizeof(char *) * new_size); // TODO error checking
    darray->allocated_size = new_size;
}

void insertToDynamicArray(DynamicArray * darray, char * data) {
    darray->content[darray->used_size] = strdup(data); // TODO error checking
    darray->used_size++;

    if(darray->used_size == darray->allocated_size) {
        resizeDynamicArray(darray);
    }
}

void freeDynamicArrayContent(DynamicArray * darray) {
    for(size_t i = 0; i < darray->used_size; i++) {
        free(darray->content[i]);
    }

    free(darray->content);
}