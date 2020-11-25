#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "view.h"
#include "buffer.h"

int gui_main();
void open_float_menu(int xpos, int ypos, const char** options, int n_options, void(*callback)(int));

const int N_SELECTABLE_VIEWS = 1;

Buffer& get_buffer(int buffer_idx);
std::vector<Buffer>& get_buffers();
Buffer* get_main_buffer();
View** get_selectable_views();
void open_file();

#endif
