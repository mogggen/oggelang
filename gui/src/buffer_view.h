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
    Buffer* buffer;
    Font* font;
    int line_num_width;
    int first_visible_line;

    void mouse_scroll_update(int scroll, Point mouse_pos);

    void draw(Window* window, Area* area);
};

void create_buffer_view(BufferView* view, Font* font);
void set_buffer(BufferView* view, Buffer* buffer);

#endif /* BUFFER_VIEW_H */
