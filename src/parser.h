#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "file_reader.h"

enum TokenType
{
    ADD,
    SUB,
    MULT,
    DIV,
    LSHIFT,
    RSHIFT,
    AND,
    OR,
    XOR,
    NOT,
    CMP,

    GOTO,
    IFGOTO,
    MOVE,
    PRINT,
    PRINTC,

    SET,
    IDENTIFIER,
    ARRAY_BEGIN,
    ARRAY_END,
    VALUE,
    FILE_LOCATION,
    IFCONDITION,
};



struct Token
{
    TokenType type;
    union{ int value; char* data; };
    FileLocation loc;
};

std::vector<Token> tokenize(FileReader& fr);

#endif
