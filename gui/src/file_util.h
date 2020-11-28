#ifndef FILE_UTIL_H
#define FILE_UTIL_H 

#include "window.h"

void convert_to_unix_file_path(char* path);
const char* get_filename_form_path(const char* file_path);
long long get_last_modified_time(const char* filename);
bool get_open_file_path(Window* window, char* file_path_buf, int buf_size);

#endif /* FILE_UTIL_H */
