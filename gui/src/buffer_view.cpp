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
    memcpy(view->buffer, error_str, len+1);
    view->buffer[len] = '\n';
    view->buffer_size = (long)len;

    view->lines.push_back(view->buffer);
    view->first_visible_line = 0;
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
        memcpy(view->buffer, error_str, len+1);
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


    // calculate the maximum width of the largest line number
    int n_digits = 0;
    int n_lines = view->lines.size();
    while(n_lines > 0)
    {
        n_digits++;
        n_lines /= 10;
    }
    char c[] = "0"; 
    int char_width, char_height;
    get_text_size(view->font, c, &char_width, &char_height);
    
    view->line_num_width = n_digits * char_width;
    view->first_visible_line = 0;
}

void draw_buffer_view(Window* window, BufferView* view)
{
    int text_xpos = view->pos.x + view->line_num_width + 2*LINE_NUM_PADDING;
    int line_num_xpos = LINE_NUM_PADDING;
    int y_pos = view->pos.y + view->font->size;

    draw_rect_fill(window, COLOR_DARK2, Point{view->pos.x + text_xpos, view->pos.y + view->height}, view->pos);

    char num_string[16];
    for(int i=view->first_visible_line; i < view->lines.size(); i++)
    {
        char* line = view->lines[i];
        sprintf(num_string, "%d", i+1);
        draw_text(window, view->font, num_string, Point{line_num_xpos, y_pos}, '\0', COLOR_BRIGHT_BLUE);
        draw_text(window, view->font, line, Point{text_xpos, y_pos}, '\n', COLOR_LIGHT);
        y_pos += view->font->size;
    }
}

void scroll_update_buffer_view(BufferView* view, int scroll)
{
    view->first_visible_line -= scroll*3; 
    if(view->first_visible_line < 0)
        view->first_visible_line = 0;
    if(view->first_visible_line > view->lines.size()-1)
        view->first_visible_line = view->lines.size()-1;
}

void close_buffer(BufferView* view)
{
    free(view->buffer);
}
