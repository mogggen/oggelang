#include "buffer_view.h"

#include <stdio.h>
#include <stdlib.h>

void create_buffer_view(BufferView* view, Point pos, int width, int height, Font* font)
{
    view->pos = pos;
    view->width = width;
    view->height = height;

    view->font = font;

    view->filename = nullptr;

    char error_str[] = "Press O to open file";
    auto len = strlen(error_str);
    view->buffer = (char*)malloc(len+1);
    strcpy_s(view->buffer, len+1, error_str);
    view->buffer[len] = '\n';
    view->buffer_size = (long)len;

    view->lines.push_back(view->buffer);
}

void open_buffer(BufferView* view, const char* filename)
{
    view->filename = filename;

    FILE* file;
    auto error_no = fopen_s(&file, filename, "r");
    if(error_no != 0)
    {
        printf("Failed to open file %s\n", filename);
        char error_str[] = "Failed to open file";
        auto len = strlen(error_str);
        view->buffer = (char*)malloc(len+1);
        strcpy_s(view->buffer, len+1, error_str);
        view->buffer[len] = '\0';
        view->buffer_size = (long)len;
        return;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    view->buffer = (char*)malloc(file_size+1);
    view->buffer[file_size] = '\0';
    view->buffer_size = file_size;

    fread(view->buffer, 1, file_size, file);

    fclose(file);

    view->lines.clear();
    view->lines.push_back(view->buffer);
    for(int i=0; i<view->buffer_size; i++)
    {
        if( view->buffer[i] == '\n' && i+1 < view->buffer_size)
            view->lines.push_back(view->buffer+i+1);
    }
}

void draw_buffer_view(Window* window, BufferView* view)
{
    Point p = view->pos;
    p.y += view->font->size;

    for(char* line : view->lines)
    {
        draw_text(window, view->font, line, p, '\n', COLOR_BRIGHT_YELLOW);
        p.y += view->font->size;
    }
}

void close_buffer(BufferView* view)
{
    free(view->buffer);
}
