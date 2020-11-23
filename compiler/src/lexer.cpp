#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "util.h"

#define  GOTO_STRING "goto"
#define  IF_STRING "if"
#define  VAR_STRING "var"
#define  ALLOC_STRING "alloc"
#define  PRINT_STRING "print"
#define  PRINTC_STRING "printc"
#define  SCAN_STRING "scan"

bool create_lexer(LexerContext* ctx, const char* filename, const char* path, BlockAlloc* symbol_names_alloc)
{
    ctx->file = fopen(path, "r");
    if(ctx->file == nullptr)
        return false;

    ctx->loc = FileLocation{filename, 1, 1};
    ctx->symbol_names_alloc = symbol_names_alloc;

    fetch_token(*ctx); // get current_token

    return true;
}

bool is_token(char c)
{
    switch((TokenType)c)
    {
        case TokenType::ADD:
        case TokenType::SUB:
        case TokenType::MULT:
        case TokenType::DIV:
        case TokenType::AND:
        case TokenType::OR:
        case TokenType::XOR:
        case TokenType::NOT:
        case TokenType::DERF:
        case TokenType::ASSIGN:
        case TokenType::LIST_BEGIN:
        case TokenType::LIST_END:
        case TokenType::COLON:
        case TokenType::LESS:
        case TokenType::GREATER:
        case TokenType::EXCLAMATION:
            return true;
        default:
            return false;
    }
}

inline bool is_white_space(char c)
{
    return c == ' ' || c == '\t';
}

inline bool is_number(char c)
{
    return '0' <= c && c <= '9';
}

int parse_uint(const char* str)
{
    int i = 0; 
    char c;
    while((c = *str++))
    {
        if(is_number(c))
        {
            i *= 10;
            i += c - '0';
        }
        else
        {
            return -1;
        }
    }

    return i;
}

Token fetch_token(LexerContext& ctx)
{
    Token new_token;
    new_token.type = TokenType::INVALID_TOKEN;
    new_token.loc = ctx.loc;

    char buf[128];
    int buf_size = 0;
    bool first_is_white_space = false; 

    while(true)
    {
        char c = fgetc(ctx.file);

        if( c == EOF )
        {
            if(buf_size == 0)
            {
                new_token.type = TokenType::NO_TOKEN;
                break;
            }
            else
            {
                buf[buf_size] = '\0';
                break;
            }
        }
        if( is_token(c) )
        {
            if( buf_size == 0 )
            {
                new_token.type = (TokenType)c;
                ctx.loc.column++;
                break;
            }
            else
            {
                ungetc(c, ctx.file);
                buf[buf_size] = '\0';
                break;
            }
        }
        else if( is_white_space(c) )
        {
            if(buf_size == 0)
            {
                first_is_white_space = true;
                ctx.loc.column++;
            }
            else
            {
                ungetc(c, ctx.file);
                buf[buf_size] = '\0';
                break;
            }
        }
        else if( c == '\n' )
        {
            if(buf_size == 0)
            {
                new_token.type = TokenType::NEW_LINE;
                ctx.loc.line++;
                ctx.loc.column = 1;
                break;
            }
            else
            {
                ungetc(c, ctx.file);
                buf[buf_size] = '\0';
                break;
            }
        }
        else
        {
            buf[buf_size++] = c;
            ctx.loc.column++;
        }
    }

    if( buf_size > 0 )
    {
        unsigned long hash = hash_djb2(buf);
        switch(hash)
        {
            case hash_djb2(GOTO_STRING):    new_token.type = TokenType::GOTO;   break;
            case hash_djb2(IF_STRING):      new_token.type = TokenType::IF;     break;
            case hash_djb2(VAR_STRING):     new_token.type = TokenType::VAR;    break;
            case hash_djb2(ALLOC_STRING):   new_token.type = TokenType::ALLOC;  break;
            case hash_djb2(PRINT_STRING):   new_token.type = TokenType::PRINT;  break;
            case hash_djb2(PRINTC_STRING):  new_token.type = TokenType::PRINTC; break;
            case hash_djb2(SCAN_STRING):    new_token.type = TokenType::SCAN; break;
            default:
            {
                if( is_number(buf[0]) )         
                {
                    new_token.type = TokenType::CONSTANT;
                    int v = parse_uint(buf);
                    if(v >= 0)
                        new_token.value = v;
                    else
                        report_error("Interger constant may not contain letters.", new_token.loc);
                }
                else
                {
                    new_token.type = TokenType::IDENTIFIER;
                    int len = strlen(buf);
                    new_token.data = (char*)allocate(*ctx.symbol_names_alloc, len+1);
                    memcpy(new_token.data, buf, len+1);
                }
            } break;
        }
    }

    Token t = ctx.current_token;
    ctx.current_token = new_token;
    return t;
}

const Token& peek_token(LexerContext& ctx)
{
    return ctx.current_token;
}

void goto_next_newline(LexerContext& lexer)
{
    Token t;
    do
    {
        t = fetch_token(lexer);
    }
    while(t.type != TokenType::NEW_LINE && t.type != TokenType::NO_TOKEN);
}

void release_lexer(LexerContext& ctx)
{
    fclose(ctx.file);
}

void print_token(Token t)
{
    switch(t.type)
    {
        case TokenType::NEW_LINE: printf("NEW_LINE\n"); break;
        case TokenType::LSHIFT: printf("LSHIFT\n"); break;
        case TokenType::RSHIFT: printf("RSHIFT\n"); break;
        case TokenType::GOTO: printf("GOTO\n"); break;
        case TokenType::IF: printf("IF\n"); break;
        case TokenType::VAR: printf("VAR\n"); break;
        case TokenType::ALLOC: printf("ALLOC\n"); break;
        case TokenType::PRINT: printf("PRINT\n"); break;
        case TokenType::PRINTC: printf("PRINTC\n"); break;
        case TokenType::SCAN: printf("SCAN\n"); break;
        case TokenType::IDENTIFIER: printf("IDENTIFIER: %s\n", t.data); break;
        case TokenType::CONSTANT: printf("CONSTANT: %d\n", t.value); break;
        case TokenType::NO_TOKEN: printf("NO_TOKEN\n"); break;
        case TokenType::INVALID_TOKEN: printf("INVALID_TOKEN\n"); break;
        default: printf("'%c'\n", (char)t.type); break;
    }
}
