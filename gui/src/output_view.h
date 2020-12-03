#ifndef OUTPUT_VIEW_H
#define OUTPUT_VIEW_H 

#include "view.h"
#include <vector>
#include "block_alloc.h"


struct _TextBlock
{
    _TextBlock* next;
    int size;
    char data[1];
};

class OutputView : public View
{
public:

    void draw(Window* window, Area* area);

    const char* name() const {return "Output";};
};

void create_output_view(OutputView* view);
void print_char(OutputView* view, char c);
void print_int(OutputView* view, int i);
void clear(OutputView* view);
void close(OutputView* view);

#endif /* OUTPUT_VIEW_H */
