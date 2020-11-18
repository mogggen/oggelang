#ifndef BUTTONS_H
#define BUTTONS_H

#include <vector>
#include "window.h"
#include "font.h"

struct Button
{
    int top,bottom,left,right;
    bool border;
    const char* text;

    void (*click_callback)(void*, void*);

    void* callback_data;

    Point text_offset;
    bool is_hovered = false;
};

struct ButtonGroup
{
    Font* font;
    Point pos;
    std::vector<Button> buttons;

    void* callback_owner;
};

Button* new_button(ButtonGroup* group,
        const char* text,
        Point pos,
        int width, int height, bool border,
        void (*click_callback)(void*,void*),
        void* callback_data);

void clear_buttons(ButtonGroup* buttons);
void draw_buttons(ButtonGroup* group, Window* window, Point pos);
void check_click(ButtonGroup* group, Point p);
void check_enter(ButtonGroup* group, Point p);

#endif
