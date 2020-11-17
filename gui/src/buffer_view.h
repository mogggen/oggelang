#ifndef BUFFER_VIEW_H
#define BUFFER_VIEW_H 

#include "window.h"
#include "buffer.h"
#include "font.h"
#include <vector>

const int LINE_NUM_PADDING = 3;

struct BufferView
{
    Buffer* buffer;
    Font* font;
    int line_num_width;
};

void create_buffer_view(BufferView* view, Font* font);
void set_buffer(BufferView* view, Buffer* buffer);
void draw_buffer_view(Window* window, BufferView* view, Area* area);
void scroll_update_buffer_view(BufferView* view, int scroll);

#endif /* BUFFER_VIEW_H */
