#include "error_view.h"
#include "error.h"
#include "font.h"
#include "file_util.h"

const int TEXT_PADDING = 5;

static ErrorView* current_error_view;


void error_handle(const char* str, FileLocation loc)
{
    char* err_string = (char*)allocate(current_error_view->error_alloc, 256);
    sprintf(err_string, "ERROR: %s %s:%d:%d", str, get_filename_form_path(loc.filename), loc.line, loc.column);
    current_error_view->errors.push_back(Error{loc, err_string});
}

void create_error_view(ErrorView* view)
{
    current_error_view = view;
    set_handle_error_func( error_handle );
    view->error_alloc = create_block_alloc(1024);
}

void ErrorView::draw(Window* window, Area* area)
{
    auto font = get_regular_font();
    Point pos = Point{area->x, area->y};
    
    int y_pos = 0;

    // fill background
    draw_rect_fill(window, COLOR_DARK, *area);

    if(this->errors.size() > 0)
    {
        for(auto error : this->errors)
        {
            draw_text(window, font, error.err_string, pos + Point{TEXT_PADDING,y_pos}, '\0', COLOR_BRIGHT_RED);
            y_pos += font->size;
        }
    }
    else
    {
       draw_text(window, font, "No Errors", pos + Point{TEXT_PADDING,y_pos});
    }
}

void clear_errors(ErrorView* view)
{
    view->errors.clear();
    dealloc(view->error_alloc);
    view->error_alloc = create_block_alloc(1024);
}

void close(ErrorView* view)
{
    dealloc(view->error_alloc);
}
