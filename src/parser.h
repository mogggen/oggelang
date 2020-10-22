#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "file_reader.h"

enum class TokenType
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
    VAR,
    MOVE,
    PRINT,
    PRINTC,

    IDENTIFIER,
    ARRAY_BEGIN,
    ARRAY_END,
    VALUE,
    PROGRAM_LOCATION,
    IFCONDITION,
};

const int EQUAL   = 0;
const int NEQUAL  = 1;
const int LEQUAL  = 2;
const int GEQUAL  = 3;
const int LESS    = 4;
const int GREAT   = 5;

struct Token
{
    TokenType type;
    union{ int value; char* data; };
    FileLocation loc;
};

std::vector<Token> tokenize(FileReader& fr);
void parse(std::vector<Token> tokens);

void print_token(Token t);

#endif
