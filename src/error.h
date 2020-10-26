#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "file_location.h"

static int num_error = 0;

inline void report_error(const char* err, FileLocation loc)
{
    printf("ERROR: %s Line %d Column %d\n", err, loc.line, loc.column);
    num_error++;
}

#endif
