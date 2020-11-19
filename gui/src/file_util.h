#ifndef FILE_UTIL_H
#define FILE_UTIL_H 

#include "window.h"

const char* get_filename_form_path(const char* file_path);
bool get_open_file_path(Window* window, char* file_path_buf, int buf_size);

#endif /* FILE_UTIL_H */
