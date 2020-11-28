#include "error.h"

static int num_error;

void default_handle_error(const char* err, FileLocation loc)
{
    printf("ERROR: %s Line %d Column %d\n", err, loc.line, loc.column);
}

void(*handle_error)(const char* err, FileLocation loc) = default_handle_error;


void report_error(const char* err, FileLocation loc)
{
    handle_error(err, loc);
    num_error++;
}

void set_handle_error_func(void(*handle)(const char* err, FileLocation loc))
{
    handle_error = handle;
}

int get_num_error()
{
    return num_error;
}
