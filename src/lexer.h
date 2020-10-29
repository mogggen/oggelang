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
    COLON = ':',
    LESS = '<',
    GREATER = '>',
    EXCLAMATION = '!',

    NEW_LINE = '\n',
    
    LSHIFT,
    RSHIFT,
    
    GOTO,
    IF,
    VAR,
    ALLOC,
    PRINT,
    PRINTC,

    IDENTIFIER,
    CONSTANT,

    NO_TOKEN,
    INVALID_TOKEN,
};

struct Token
{
    TokenType type;
    FileLocation loc;
    union { int value; char* data; };
};

struct LexerContext
{
    FILE* file;
    FileLocation loc;
    Token current_token;
};

bool create_lexer(LexerContext* ctx, const char* filename);
Token fetch_token(LexerContext& ctx);
const Token& peek_token(LexerContext& ctx);
void release_lexer(LexerContext& ctx);

void goto_next_newline(LexerContext& lexer);

void print_token(Token t);

#endif
