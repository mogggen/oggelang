#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H 

#include "window.h"
#include "buttons.h"
#include "font.h"

const int CONTROL_BAR_HEIGHT = 24;

struct ControlBar
{
    ButtonGroup buttons;
};

void create_control_bar(Window* window, ControlBar* bar, Font* font);
void draw_control_bar(Window* window, ControlBar* bar);

#endif /* CONTROL_BAR_H */
