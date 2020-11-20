#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "buffer.h"

int gui_main();
void open_float_menu(int xpos, int ypos, const char** options, int n_options, void(*callback)(int));

Buffer& get_buffer(int buffer_idx);
std::vector<Buffer>& get_buffers();
Buffer* get_main_buffer();
void open_file();

#endif
