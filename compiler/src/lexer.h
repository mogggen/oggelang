#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "file_location.h"
#include "block_alloc.h"

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
    LIST_BEGIN = '[',
    LIST_END = ']',
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
    SCAN,

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
    BlockAlloc* symbol_names_alloc;
};

bool create_lexer(LexerContext* ctx, const char* filename, const char* path, BlockAlloc* symbol_names_alloc);
Token fetch_token(LexerContext& ctx);
const Token& peek_token(LexerContext& ctx);
void release_lexer(LexerContext& ctx);

void goto_next_newline(LexerContext& lexer);

void print_token(Token t);

#endif
