#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "file_location.h"

void report_error(const char* err, FileLocation loc);
int get_num_error();

#endif
