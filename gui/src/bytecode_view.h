#ifndef BYTECODE_VIEW_H
#define BYTECODE_VIEW_H 

#include <vector>
#include "block_alloc.h"
#include "window.h"
#include "view.h"
#include "font.h"
#include "opcodes.h"

class ByteCodeView : public View
{
public:
    Point pos;
    int line_num_width;
    int first_visible_line;
    bool has_bytecode = false;
    
    std::vector<char*> lines;
    std::vector<int> addresses;
    BlockAlloc line_alloc;

    void mouse_scroll_update(int scroll, Point mouse_pos);

    void draw(Window* window, Area* area);
};

void create_bytecode_view(ByteCodeView* view);
void show_bytecode(ByteCodeView* view, ByteCode* byte_code);

#endif /* BYTECODE_VIEW_H */
