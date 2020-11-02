#include "error.h"

static int num_error;

void report_error(const char* err, FileLocation loc)
{
    printf("ERROR: %s Line %d Column %d\n", err, loc.line, loc.column);
    num_error++;
}

int get_num_error()
{
    return num_error;
}
