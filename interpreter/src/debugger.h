#ifndef DEBUGGER_H
#define DEBUGGER_H 

#include <vector>
#include "opcodes.h"
#include "stack.h"

void default_report_error(const char* err_str, int addr);
void default_print_char(int c);
void default_print_int(int i);
int  default_input_int();

struct DebugState
{
    int pc;
    Stack stack;
    std::vector<int> heap;
    ByteCode* code;
    DebugInfo* dbginfo;

    void(*report_error_func)( const char* err_str, int addr) = default_report_error;
    void(*print_char_func)(int) = default_print_char;
    void(*print_int_func)(int) = default_print_int;
    int (*input_int_func)() = default_input_int;
};

void start_debug(DebugState* dbgstate, ByteCode* code, DebugInfo* dbginfo);
void step_instruction(DebugState* dbgstate);
void step_line(DebugState* dbgstate);
void run(DebugState* dbgstate);

#endif /* DEBUGGER_H */
