#ifndef BUFFER_VIEW_H
#define BUFFER_VIEW_H 

#include "window.h"
#include "font.h"
#include <vector>

struct BufferView
{
    Point pos;
    int width, height;
    const char* filename;
    Font* font;
    char* buffer;    
    long buffer_size;
    std::vector<char*> lines;
};

void create_buffer_view(BufferView* view, Point pos, int width, int height, Font* font);
void open_buffer(BufferView* view, const char* filename);
void draw_buffer_view(Window* window, BufferView* view);
void close_buffer(BufferView* view);

#endif /* BUFFER_VIEW_H */
