#ifndef BUFFER_VIEW_H
#define BUFFER_VIEW_H 

#include "view.h"
#include "window.h"
#include "buffer.h"
#include "font.h"
#include <vector>

const int LINE_NUM_PADDING = 3;

class BufferView : public View
{
public:
    int buffer_idx;
    Font* font;
    int line_num_width;
    int first_visible_line;

    void mouse_scroll_update(int scroll, Point mouse_pos);
    void mouse_right_click(Point mouse_pos);

    void draw(Window* window, Area* area);
};

void create_buffer_view(BufferView* view, Font* font);
void set_buffer(BufferView* view, int buffer_idx);

#endif /* BUFFER_VIEW_H */
