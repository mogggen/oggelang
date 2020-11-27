#ifndef VARIABLE_VIEW_H
#define VARIABLE_VIEW_H 

#include "view.h"
#include "opcodes.h"
#include "debug_info.h"

class VariableView : public View
{
public:
    ByteCode* bytecode;
    DebugInfo* dbginfo;

    void draw(Window* window, Area* area);
    
    const char* name() const {return "Variables";};
};

void create_variable_view(VariableView* view, ByteCode* bytecode, DebugInfo* dbginfo);

#endif /* VARIABLE_VIEW_H */
