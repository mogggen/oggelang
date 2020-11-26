#ifndef VIEW_SELECT_H
#define VIEW_SELECT_H 

#include "view.h"
#include "buffer_view.h"

class ViewSelect : public View
{
public:
    View* inner;
    Area button_area;

    BufferView buffer_view;
    
    
    void mouse_scroll_update(int scroll, Point mouse_pos);
    void mouse_left_click(Point mouse_pos);
    void mouse_right_click(Point mouse_pos);
    void mouse_enter(Point mouse_pos);
    
    void draw(Window* window, Area* area);
};
    
void create_view_select(ViewSelect* view);
void create_view_select(ViewSelect* view, View* inner);

#endif /* VIEW_SELECT_H */
