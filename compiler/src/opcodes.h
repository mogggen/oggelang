#ifndef OPCODES_H
#define OPCODES_H

#include "debug_info.h"

enum class OpCode
{
    PUSHC,
    PUSHV,
    POP,
    COPY,

    ADD,
    SUB,
    MULT,
    DIV,
    AND,
    OR,
    XOR,
    CMP,
    NOT,
    LSHIFT,
    RSHIFT,
    
    REF,
    DERF,
    MOVE,
    MOVED,
    ALLOC,

    GOTO,
    IF,
    PRINT,
    PRINTC,
    SCAN,

    END,
};

enum class CmpTypes
{
    EQUAL,
    NOT_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS,
    GREATER,
};

struct ByteCode
{
    int size;
    int code_size;
    int* data;
};

void print_opcodes(ByteCode code);
void print_opcodes(ByteCode code, DebugInfo* dbginfo);

#endif
