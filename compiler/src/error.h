#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "file_location.h"

void report_error(const char* err, FileLocation loc);
void set_handle_error_func(void(*handle)(const char* err, FileLocation loc));
int get_num_error();
void reset_num_error();

#endif
