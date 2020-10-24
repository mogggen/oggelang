#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "file_location.h"

enum class TokenType
{
    ADD = '+',
    SUB = '-',
    MULT = '*',
    DIV = '/',
    AND = '&',
    OR = '|',
    XOR = '^',
    NOT = '~',
    REF = '*',
    DERF = '@',
    ASSIGN = '=',
    ARRAY_BEGIN = '[',
    ARRAY_END = ']',
    EQUALS = '=',

    NEW_LINE = '\n',
    WHITE_SPACE,
    
    LSHIFT,
    RSHIFT,
    
    GOTO,
    IF,
    VAR,
    ALLOC,
    PRINT,
    PRINTC,

    IDENTIFIER,
    VALUE,
    PROGRAM_LOCATION,

    NO_TOKEN
};

struct Token
{
    TokenType type;
    FileLocation loc;
    char* data;
};

struct LexerContext
{
    FILE* file;
    FileLocation loc;
    Token current_token;
};

bool create_lexer(LexerContext* ctx, const char* filename);
Token fetch_token(LexerContext& ctx);
Token peek_token(LexerContext& ctx);
void release_lexer(LexerContext& ctx);

#endif
