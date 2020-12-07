#include "output_view.h"
#include "font.h"

void OutputView::draw(Window* window, Area* area)
{
    // fill background
    draw_rect_fill(window, COLOR_DARK, *area);

    _TextBlock* curr = &this->first;

    DrawTextCursor cursor = create_cursor(get_regular_font(), Point{area->x+2, area->y+2});

    while(curr != nullptr)
    {
        for(int i=0; i<curr->size; i++)
        {
            char c = curr->data[i];

            if(c == '\n')
            {
                next_line(&cursor);
                continue;
            }

            draw_char(window, &cursor, c, COLOR_LIGHT);
        }

        curr = curr->next;
    }
}

void create_output_view(OutputView* view)
{
    view->current = &view->first; 
    view->first.data[0] = '\0';
}

void print_char(OutputView* view, char c)
{
    if(view->current->size + 1 >= TEXT_BLOCK_SIZE-1) // -1 to make space for nullterminator
    {
        // create new text block
        view->current->next = (_TextBlock*)malloc(sizeof(_TextBlock));
        view->current = view->current->next;
        view->current->size = 0;
        view->current->next = nullptr;
    }

    view->current->data[view->current->size] = c;
    view->current->size += 1;
    view->current->data[view->current->size] = '\0';
}

void print_int(OutputView* view, int i)
{
    int n_chars = 0;
    char buf[10]; // largest int only requires 10 chars to be printed in decimal

    if(i < 0)
    {
        print_char(view, '-');
        i = abs(i);
    }

    while(i > 0)
    {
        buf[n_chars++] = i%10 + '0';
        i /= 10;
    }

    while(n_chars >= 0)
        print_char(view, buf[--n_chars]);
}

void clear(OutputView* view)
{
    _TextBlock* curr = view->first.next;

    while(curr != nullptr)
    {
        _TextBlock* t = curr->next;
        free(curr);
        curr = t;
    }
}

void close(OutputView* view)
{
    clear(view);
}
