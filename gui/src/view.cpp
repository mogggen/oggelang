#include "view.h"
#include "window.h"
#include "gui_main.h"

HSplit::HSplit(View* top, View* bottom, float height)
    : top(top), bottom(bottom), split_height(0), absolut(false), height(height)
{ }
HSplit::HSplit(View* top, View* bottom, int split_height)
    : top(top), bottom(bottom), split_height(split_height), absolut(true), height(0.0)
{ }

void HSplit::mouse_scroll_update(int scroll, Point mouse_pos)
{
    if(mouse_pos.y < this->split_height)
        top->mouse_scroll_update(scroll, mouse_pos);
    else
        bottom->mouse_scroll_update(scroll, mouse_pos);
}

void HSplit::mouse_left_click(Point mouse_pos)
{
    if(mouse_pos.y < this->split_height)
        top->mouse_left_click(mouse_pos);
    else
        bottom->mouse_left_click(mouse_pos);
}
void HSplit::mouse_right_click(Point mouse_pos)
{
    if(mouse_pos.y < this->split_height)
        top->mouse_right_click(mouse_pos);
    else
        bottom->mouse_right_click(mouse_pos);
}
void HSplit::mouse_enter(Point mouse_pos)
{
    if(mouse_pos.y < this->split_height)
        top->mouse_enter(mouse_pos);
    else
        bottom->mouse_enter(mouse_pos);
}
void HSplit::draw(Window* window, Area* area)
{
    if(!this->absolut)
        this->split_height = area->height*this->height;

    Area atop = *area;
    atop.height = this->split_height - atop.y;
    top->draw(window, &atop);
    
    Area abottom = *area;
    abottom.y = this->split_height;
    bottom->draw(window, &abottom);

    draw_line(window, COLOR_LIGHT, Point{area->x, this->split_height}, Point{area->x+area->width, this->split_height});
}

VSplit::VSplit(View* left, View* right, float width)
    : left(left), right(right), split_width(0), absolut(false), width(width)
{ }
VSplit::VSplit(View* left, View* right, int split_width)
    : left(left), right(right), split_width(split_width), absolut(true), width(0.0)
{ }

void VSplit::mouse_scroll_update(int scroll, Point mouse_pos)
{
    if(mouse_pos.x < this->split_width)
        left->mouse_scroll_update(scroll, mouse_pos);
    else
        right->mouse_scroll_update(scroll, mouse_pos);
}

void VSplit::mouse_left_click(Point mouse_pos)
{
    if(mouse_pos.x < this->split_width)
        left->mouse_left_click(mouse_pos);
    else
        right->mouse_left_click(mouse_pos);
}
void VSplit::mouse_right_click(Point mouse_pos)
{
    if(mouse_pos.x < this->split_width)
        left->mouse_right_click(mouse_pos);
    else
        right->mouse_right_click(mouse_pos);
}
void VSplit::mouse_enter(Point mouse_pos)
{
    if(mouse_pos.x < this->split_width)
        left->mouse_enter(mouse_pos);
    else
        right->mouse_enter(mouse_pos);
}
void VSplit::draw(Window* window, Area* area)
{
    if(!this->absolut)
        this->split_width = area->width*this->width;

    Area aleft = *area;
    aleft.width = this->split_width - aleft.x;
    left->draw(window, &aleft);
    
    Area aright = *area;
    aright.x = this->split_width;
    right->draw(window, &aright);

    draw_line(window, COLOR_LIGHT, Point{this->split_width, area->y}, Point{this->split_width, area->y+area->height});
}
    
ViewSelect::ViewSelect(View* inner)
    : inner(inner)
{}
    
void ViewSelect::mouse_scroll_update(int scroll, Point mouse_pos)
{
    inner->mouse_scroll_update(scroll, mouse_pos);
}

void ViewSelect::mouse_left_click(Point mouse_pos)
{
    if(in_area(&button_area, mouse_pos))
    {}
    else
        inner->mouse_left_click(mouse_pos);
    
}
void ViewSelect::mouse_right_click(Point mouse_pos)
{
    inner->mouse_right_click(mouse_pos);
}
void ViewSelect::mouse_enter(Point mouse_pos)
{
    inner->mouse_enter(mouse_pos);
}

void ViewSelect::draw(Window* window, Area* area)
{
    inner->draw(window, area);
    button_area = Area{
        area->width-15,
        area->y+5,
        10,
        10
    };
    draw_rect_fill(window, COLOR_BRIGHT_PURPLE, Point{button_area.x + button_area.width, button_area.y + button_area.height}, Point{button_area.x, button_area.y});
}
