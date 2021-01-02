#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include "view.h"
#include "buffer.h"

int gui_main();

const int N_SELECTABLE_VIEWS = 4;

Buffer& get_buffer(int buffer_idx);
std::vector<Buffer>& get_buffers();
Buffer* get_main_buffer();
View** get_selectable_views();

int get_pc();

void open_file();
void run_program();
void step_program();
void stop_program();

#endif
