#include <time.h>

#ifndef MULTIPLATFORM_IO
#define MULTIPLATFORM_IO

#ifdef _WIN32
    #include <io.h>
#elif __linux__
    #include <inttypes.h>
    #include <unistd.h>
    #define __int64 int64_t
    #define _close close
    #define _read read
    #define _lseek64 lseek64
    #define _O_RDONLY O_RDONLY
    #define _open open
    #define _lseeki64 lseek64
    #define _lseek lseek
    #define stricmp strcasecmp

    //! FIXME
    #define _MAX_FNAME 300

    typedef struct _finddata_t {
        unsigned attrib;
        time_t time_create;
        time_t time_access;
        time_t time_write;
        long long size; // ?
        char name[_MAX_FNAME];
    };

#endif

#endif