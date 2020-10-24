#ifndef OPCODES_H
#define OPCODES_H

enum class OpCode
{
    ADD,
    SUB,
    MULT,
    DIV,
    AND,
    OR,
    XOR,
    CMP,
    MOVE,

    ADDC,
    SUBC,
    MULTC,
    DIVC,
    ANDC,
    ORC,
    XORC,
    CMPC,
    MOVEC,

    MOVER,
    MOVERC,

    REF,
    DERF,

    ALLOC,
    NOT,
    LSHIFT,
    RSHIFT,

    GOTO,
    IFGOTO,

    PRINT,
    PRINTC,
};

struct ByteCode
{
    int size;
    int code_size;
    int* data;
    int* line_numbers;
};

#endif
