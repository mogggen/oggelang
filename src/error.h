#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "file_location.h"

inline void report_error(const char* err, FileLocation loc)
{
    printf("ERROR: %s Line %d Column %d\n", err, loc.line, loc.column);
}

#endif
