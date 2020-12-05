#include "output_view.h"

void OutputView::draw(Window* window, Area* area)
{
}

void create_output_view(OutputView* view)
{
    view->current = &view->first; 
}

char* add_text(OutputView* view, int size)
{
    if(view->current->size + size >= TEXT_BLOCK_SIZE)
    {
        // create new text block
    }

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
