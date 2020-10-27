#ifndef OPCODES_H
#define OPCODES_H

enum class OpCode
{
    PUSHC,
    PUSHV,
    POP,

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
};

struct ByteCode
{
    int size;
    int code_size;
    int* data;
};

#endif
