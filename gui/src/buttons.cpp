#include "buttons.h"

void clear_buttons(ButtonGroup* buttons)
{
    buttons->buttons.clear();
}

Button* new_button(ButtonGroup* group, const char* text, Point pos, int width, int height, bool border, void (*click_callback)(void*,void*), void* callback_data)
{
    Button b;
    b.text = text;
    b.left = pos.x;
    b.top = pos.y;
    b.border = border;
    b.click_callback = click_callback;
    b.callback_data = callback_data;

    int text_width, text_height;
    get_text_size(group->font, b.text, &text_width, &text_height);
    
    if(width <= 0)
        b.right = b.left + text_width + 8;
    else
        b.right = b.left + width;
    
    if(height <= 0)
        b.bottom = b.top + text_height + 8;
    else
        b.bottom = b.top + height;

    //printf("%d %d\n", text_height, b.bottom);

    b.text_offset = 
    {
        ((b.right - b.left) - text_width )/2,
        ((b.bottom - b.top) + text_height)/2
    };

    group->buttons.push_back(b);
    return &group->buttons.back();
}

void draw_buttons(ButtonGroup* group, Window* window, Point pos)
{
    group->pos = pos;

    for(auto b : group->buttons)
    {
        Point max = Point{pos.x + b.right, pos.y + b.bottom};
        Point min = Point{pos.x + b.left, pos.y + b.top};
        if(b.is_hovered)
            draw_rect_fill(window, COLOR_NEUTRAL_GREEN, max, min);
        if(b.border)
            draw_rect(window, COLOR_LIGHT, max, min);
        draw_text(window, group->font, b.text, Point{pos.x + b.left+b.text_offset.x, pos.y + b.top+b.text_offset.y});
    }
}

void check_click(ButtonGroup* group, Point pos)
{
    Point p = Point{pos.x - group->pos.x, pos.y - group->pos.y};
    for(Button& b : group->buttons)
    {
        if( b.left <= p.x && p.x <= b.right &&
            b.top <= p.y && p.y <= b.bottom)
        {
            b.click_callback(group->callback_owner, b.callback_data);
        }
    }
}

void check_enter(ButtonGroup* group, Point pos)
{
    Point p = Point{pos.x - group->pos.x, pos.y - group->pos.y};
    for(Button& b : group->buttons)
    {
        b.is_hovered = b.left <= p.x && p.x <= b.right && b.top <= p.y && p.y <= b.bottom;
    }
}
