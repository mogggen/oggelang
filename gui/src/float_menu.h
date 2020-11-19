#ifndef FLOAT_MENU_H
#define FLOAT_MENU_H 

#include "window.h"
#include "font.h"
#include "buttons.h"

const int FLOAT_MENU_PADDING = 3;

struct FloatMenu
{
    const char** options;
    int n_options;
    Area area; 
    int option_height;

    ButtonGroup buttons;

    void (*callback)(int);
};

void init_float_menu(FloatMenu* menu, Point pos, const char** options, int n_options, Font* font, int win_width, int win_height, void(*callback)(int));
void draw_float_menu(Window* window, FloatMenu* menu, Font* font);
void mouse_enter(FloatMenu* menu, Point mouse_pos);
void mouse_click(FloatMenu* menu, Point mouse_pos);

#endif /* FLOAT_MENU_H */
