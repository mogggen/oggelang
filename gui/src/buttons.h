#ifndef BUTTONS_H
#define BUTTONS_H

#include <vector>
#include "window.h"

struct Button
{
    int top,bottom,left,right;
    const char* text;
    void (*click_callback(void*));
    void* callback_data;
};

struct ButtonGroup
{
    std::vector<Button> buttons;
};

void add_button(ButtonGroup* group, Button b);
void render_buttons(ButtonGroup* group, Window* window);

#endif
