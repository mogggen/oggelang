#ifndef FRAME_H
#define FRAME_H

#include "window.h"

class View
{
public:
    virtual void mouse_scroll_update(int scroll, Point mouse_pos) {}
    virtual void mouse_left_click(Point mouse_pos) {}
    virtual void mouse_right_click(Point mouse_pos) {}
    virtual void mouse_enter(Point mouse_pos) {}
    
    virtual void draw(Window* window, Area* area) = 0;
};

class HSplit : public View
{
public:
    View* top;
    View* bottom;
    int split_height;
    bool absolut;
    float height;

    HSplit(View* top, View* bottom, float height);
    HSplit(View* top, View* bottom, int split_height);

    void mouse_scroll_update(int scroll, Point mouse_pos); 
    void mouse_left_click(Point mouse_pos);
    void mouse_right_click(Point mouse_pos);
    void mouse_enter(Point mouse_pos);
    
    void draw(Window* window, Area* area);
};

class VSplit : public View
{
public:
    View* left;
    View* right;
    int split_width;
    bool absolut;
    float width;

    VSplit(View* left, View* right, float width);
    VSplit(View* left, View* right, int split_width);

    void mouse_scroll_update(int scroll, Point mouse_pos); 
    void mouse_left_click(Point mouse_pos);
    void mouse_right_click(Point mouse_pos);
    void mouse_enter(Point mouse_pos);
    
    void draw(Window* window, Area* area);
};

class ViewSelect : public View
{
public:
    View* inner;
    Area button_area;
    
    ViewSelect(View* inner);
    
    void mouse_scroll_update(int scroll, Point mouse_pos);
    void mouse_left_click(Point mouse_pos);
    void mouse_right_click(Point mouse_pos);
    void mouse_enter(Point mouse_pos);
    
    void draw(Window* window, Area* area);
};

#endif
