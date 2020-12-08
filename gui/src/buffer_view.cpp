#include "buffer_view.h"

#include <stdio.h>
#include <stdlib.h>
#include "gui_main.h"
#include "font.h"
#include "float_menu.h"

const int LINE_NUM_PADDING = 3;

static BufferView* selected_buffer_view;

void BufferView::mouse_scroll_update(int scroll, Point mouse_pos)
{
    if(this->buffer_idx < 0)
        return;

    Buffer& buffer = get_buffer(this->buffer_idx);

    this->first_visible_line -= scroll*3; 
    if(this->first_visible_line < 0)
        this->first_visible_line = 0;
    if(this->first_visible_line > buffer.lines.size()-1)
        this->first_visible_line = (long)buffer.lines.size()-1;
}

void select_buffer_menu_callback(int i)
{
    set_buffer(selected_buffer_view, i);
}
const char* select_buffer_options[16];

void open_select_buffer_menu(BufferView* view, Point pos)
{
    if(get_buffers().size() <= 0)
        return;

    int i = 0;
    for(const Buffer& b: get_buffers())
    {
        select_buffer_options[i] = b.filename;
        i++;
        if(i>16)
            break;
    }

    open_float_menu(pos.x, pos.y, select_buffer_options, i, select_buffer_menu_callback);
}

void BufferView::mouse_left_click(Point mouse_pos)
{
    selected_buffer_view = this; 
    if(mouse_pos.y < this->pos.y + get_monospace_font()->size) // if clicked on top bar
    {
        open_select_buffer_menu(this, mouse_pos);
    }
}

void BufferView::mouse_right_click(Point mouse_pos)
{
    selected_buffer_view = this; 
    open_select_buffer_menu(this, mouse_pos);
}

void BufferView::draw(Window* window, Area* area)
{
    auto font = get_monospace_font();

    this->pos = {area->x, area->y};
    int text_xpos = this->line_num_width + 2*LINE_NUM_PADDING;
    int line_num_xpos = LINE_NUM_PADDING;
    int y_pos = font->size;

    // fill background
    draw_rect_fill(window, COLOR_DARK, *area);
    
    // draw line number bar
    draw_rect_fill(window, COLOR_DARK2, Area{area->x, area->y, text_xpos, area->height});
    
    // draw top bar
    draw_rect_fill(window, COLOR_DARK2, Area{area->x, area->y, area->width, font->size});

    if(this->buffer_idx < 0)
    {
        draw_text(window, font, "Open a file (Ctrl-O) or", this->pos+Point{text_xpos, y_pos}, '\0', COLOR_LIGHT);
        draw_text(window, font, "select an open file (Right Click)", this->pos+Point{text_xpos, y_pos+font->size}, '\0', COLOR_LIGHT);
    }
    else
    {
        Buffer& buffer = get_buffer(this->buffer_idx);
        // draw filename
        draw_text(window, font, buffer.filename, this->pos + Point{0, -3});

        // draw content
        char num_string[16];
        for(int i=this->first_visible_line; i < buffer.lines.size(); i++)
        {
            char* line = buffer.lines[i];
            sprintf(num_string, "%d", i+1);
            draw_text(window, font, num_string, this->pos+Point{line_num_xpos, y_pos}, '\0', COLOR_BRIGHT_BLUE); // draw line number
            draw_text(window, font, line, this->pos+Point{text_xpos, y_pos}, '\n', COLOR_LIGHT); // draw text
            y_pos += font->size;
        }
    }
}

void create_buffer_view(BufferView* view)
{
    view->buffer_idx = -1;
    view->line_num_width = 0;
    selected_buffer_view = view; 
}

void set_buffer(BufferView* view, int buffer_idx)
{
    view->buffer_idx = buffer_idx;
    Buffer& buffer = get_buffer(buffer_idx);

    // calculate the maximum width of the largest line number
    int n_digits = 0;
    int n_lines = buffer.lines.size();
    while(n_lines > 0)
    {
        n_digits++;
        n_lines /= 10;
    }
    char c[] = "0"; 
    int char_width, char_height;
    get_text_size(get_monospace_font(), c, &char_width, &char_height);
    
    view->line_num_width = n_digits * char_width;
    view->first_visible_line = 0;
}

BufferView* get_selected_buffer_view()
{
    return selected_buffer_view;
}
