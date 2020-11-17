#ifndef CONTROL_BAR_H
#define CONTROL_BAR_H 

#include "view.h"
#include "window.h"
#include "buttons.h"
#include "font.h"

const int CONTROL_BAR_HEIGHT = 24;

class ControlBar : public View
{
public:
    ButtonGroup buttons;

    void mouse_left_click(Point mouse_pos);
    void mouse_enter(Point mouse_pos);

    void draw(Window* window, Area* area);
};

void create_control_bar(Window* window, ControlBar* bar, Font* font);
void draw_control_bar(Window* window, ControlBar* bar);

#endif /* CONTROL_BAR_H */
