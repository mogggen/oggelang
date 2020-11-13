#include "buttons.h"

#include "font.h"

void add_button(ButtonGroup* group, Button b)
{
    group->buttons.push_back(b);
}

void render_buttons(ButtonGroup* group, Window* window)
{
    for(auto b : group->buttons)
    {
        
    }
}
