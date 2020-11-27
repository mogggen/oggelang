#include "variable_view.h"

#include "font.h"
#include "file_util.h"

const int TEXT_PADDING = 2;

void VariableView::draw(Window* window, Area* area)
{
    auto font = get_regular_font();
    Point pos = Point{area->x, area->y};
    
    int y_pos = font->size;

    // fill background
    draw_rect_fill(window, COLOR_DARK, *area);

    if(this->dbginfo->n_variables > 0)
    {
        for(int i=0; i<this->dbginfo->n_variables; i++)
        {
           const DebugVar* var = &this->dbginfo->variables[i];

           char buf[512];
           sprintf(buf, "%.5d | %s %s : %d",
                   var->addr,
                   get_filename_form_path(var->loc.filename),
                   var->name,
                   this->bytecode->data[var->addr] );
            
           draw_text(window, font, buf, pos + Point{TEXT_PADDING,y_pos});
           y_pos += font->size;
        }
    }
    else
    {
       draw_text(window, font, "No variables loaded", pos + Point{TEXT_PADDING,y_pos});
    }
}

void create_variable_view(VariableView* view, ByteCode* bytecode, DebugInfo* dbginfo)
{
    view->bytecode = bytecode;
    view->dbginfo = dbginfo;
}
