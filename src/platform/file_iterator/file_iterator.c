#include "file_iterator.h"
#include "utils/listdir.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h> //!FIXME if errno gets introduced this must be removed

#define DEFAULT_FILE_ITERATOR_DARRAY_SIZE 10

static FileIterator * fileIterator;

FileIterator * createFileIterator(const char * path, const char * filespec) {
    // TODO error handlnig
    FileIterator * fileIterator = malloc(sizeof(FileIterator));
    fileIterator->files = createDynamicArray(DEFAULT_FILE_ITERATOR_DARRAY_SIZE);
    fileIterator->path = strdup(path);
    fileIterator->filespec = strdup(filespec);

    fileIterator->current = 0;

    return fileIterator;
}

void freeFileIterator(FileIterator * fileIterator) {
    freeDynamicArrayContent(&fileIterator->files);

    free(fileIterator->path);
    free(fileIterator->filespec);
    free(fileIterator);
}

short findfirst(const char * path, const char * filespec, char * fullpath) {
    if(fileIterator != NULL) {
        // TODO use errno instead
        printf("[*] ERROR: file iterator was instantiated and yet findFirst was called");
        return 0;
    }

    fileIterator = createFileIterator(path, filespec);
    fileIterator->count = listFilesInDir(&fileIterator->files, fileIterator->path, fileIterator->filespec);

    if(!findnext(fullpath)) {
        return 0;
    }

    return 1;
}

short findnext(char * fullpath) {
    if(fileIterator == NULL) {
        // TODO use errno instead
        printf("[*] ERROR: file iterator was not instantiated yet findnext was called");
        return 0;
    }

    if(fileIterator->current == fileIterator->count) {
        return 0;
    }

    sprintf(
        fullpath, "%s/%s",
        fileIterator->path, (&fileIterator->files)->content[fileIterator->current++]
    );

    return 1;
}

void findclose() {
    if(fileIterator == NULL) {
        // TODO use errno instead
        printf("[*] ERROR: file iterator was not instantiated yet findclose was called");
        return;
    }

    freeFileIterator(fileIterator);
}