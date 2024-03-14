#pragma once

#include "dynamic_array.h"

/*
Lists filenames into a darray based on the given parameters
@param darray: dynamic array that will hold the filenames
@param dirPath: Path to a directory that we will look for files
@param pattern: WARNING: not regex! fileends that we are looking for in the given dirPath
@returns 0 if dirPath cound not be opened or found file count is 0. Other than that returns the found file amount
@todo errno to separate dirPath error and files not found
*/
int listFilesInDir(DynamicArray * darray, const char * dirPath, const char * pattern);