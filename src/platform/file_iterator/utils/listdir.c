#include "listdir.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h> //! FIXME temporary, if errno gets introduced, remove this

short endsWith(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (str_len < suffix_len) {
        return 0;
    }

    return strncmp(str + (str_len - suffix_len), suffix, suffix_len) == 0;
}

int listFilesInDir(DynamicArray * darray, const char *dirPath, const char *pattern) {
    int found = 0;
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirPath);
    if (dir == NULL) {
        printf("[*] ERROR: cound not open dir: *%s*\n", dirPath); //! FIXME
        // TODO: errno to give a sign that opening the dir was not a success
        return found;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (endsWith(entry->d_name, pattern)) {
            insertToDynamicArray(darray, entry->d_name);
            found++;
        }
    }

    closedir(dir);

    return found;
}
