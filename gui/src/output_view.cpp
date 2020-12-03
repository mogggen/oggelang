#include "output_view.h"

void OutputView::draw(Window* window, Area* area)
{
}

void create_output_view(OutputView* view)
{
    view->buffer_alloc = create_block_alloc(1024);
}

void print_char(OutputView* view, char c)
{
    
}

void print_int(OutputView* view, int i)
{

}

void clear(OutputView* view)
{

}

void close(OutputView* view)
{

}
