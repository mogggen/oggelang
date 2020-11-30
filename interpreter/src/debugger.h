#ifndef DEBUGGER_H
#define DEBUGGER_H 

#include <vector>
#include "opcodes.h"
#include "stack.h"

struct DebugState
{
    int pc;
    Stack stack;
    std::vector<int> heap;
    ByteCode* code;
    DebugInfo* dbginfo;

    void(*report_error_func)( const char* err_str, int addr);
    void(*print_char_func)(int);
    void(*print_int_func)(int);
    int (*input_int_func)();
};

void start_debug(DebugState* dbgstate, ByteCode* code, DebugInfo* dbginfo);
void step_instruction(DebugState* dbgstate);
void step_line(DebugState* dbgstate);
void run(DebugState* dbgstate);

#endif /* DEBUGGER_H */
