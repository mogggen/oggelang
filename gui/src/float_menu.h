#ifndef FLOAT_MENU_H
#define FLOAT_MENU_H 

#include "window.h"
#include "font.h"
#include "buttons.h"


void draw_float_menu(Window* window);
void float_menu_mouse_enter(Point mouse_pos);
void float_menu_mouse_click(Point mouse_pos);

void open_float_menu(int xpos, int ypos, const char** options, int n_options, void(*callback)(int));
bool is_float_menu_open();

#endif /* FLOAT_MENU_H */
