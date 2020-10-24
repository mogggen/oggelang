#include "error.h"

void report_error(const char* err, FileLocation loc)
{
    printf("ERROR: %s Line %d Column %d\n", err, loc.line, loc.column);
}
