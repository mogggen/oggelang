#ifndef BUFFER_VIEW_H
#define BUFFER_VIEW_H 

#include "view.h"
#include "window.h"
#include "buffer.h"
#include <vector>

class BufferView : public View
{
public:
    int buffer_idx;
    Point pos;
    int line_num_width;
    int first_visible_line;

    void mouse_scroll_update(int scroll, Point mouse_pos);
    void mouse_left_click(Point mouse_pos);
    void mouse_right_click(Point mouse_pos);

    void draw(Window* window, Area* area);

    const char* name() const {return "Buffer";};
};

void create_buffer_view(BufferView* view);
void set_buffer(BufferView* view, int buffer_idx);

BufferView* get_selected_buffer_view();

#endif /* BUFFER_VIEW_H */
