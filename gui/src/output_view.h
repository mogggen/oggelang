#ifndef OUTPUT_VIEW_H
#define OUTPUT_VIEW_H 

#include "view.h"
#include <vector>
#include "block_alloc.h"


const int TEXT_BLOCK_SIZE = 1024;

struct _TextBlock
{
    _TextBlock* next = nullptr;
    int size = 0;
    char data[TEXT_BLOCK_SIZE];
};

class OutputView : public View
{
public:
    _TextBlock first;
    _TextBlock* current;

    void draw(Window* window, Area* area);

    const char* name() const {return "Output";};
};

void create_output_view(OutputView* view);
void print_char(OutputView* view, char c);
void print_int(OutputView* view, int i);
void clear(OutputView* view);
void close(OutputView* view);

#endif /* OUTPUT_VIEW_H */
