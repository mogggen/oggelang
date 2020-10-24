#include "lexer.h"

#define  GOTO_STRING "goto"
#define  IF_STRING "if"
#define  VAR_STRING "var"
#define  ALLOC_STRING "alloc"
#define  PRINT_STRING "print"
#define  PRINTC_STRING "printc"

constexpr unsigned long hash_djb2(const char*str)
{
    unsigned long hash = 5381;
    int c = 0;
    while ((c = *str++))
        hash = hash*33 + c;

    return hash;
}

bool create_lexer(LexerContext* ctx, const char* filename)
{
    ctx->file = fopen(filename, "r");
    if(ctx->file == nullptr)
        return false;

    ctx->loc = FileLocation{filename, 1, 1};

    fetch_token(*ctx);

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
        case TokenType::ARRAY_BEGIN:
        case TokenType::ARRAY_END:
            return true;
        default:
            return false;
    }
}

bool is_white_space(char c)
{
    return c == ' ' || c == '\t';
}


Token fetch_token(LexerContext& ctx)
{
    Token new_token;
    new_token.type = TokenType::NO_TOKEN;
    new_token.loc = ctx.loc;

    char buf[128];
    int buf_size = 0;
    bool first_is_white_space = false; 

    while(true)
    {
        char c = fgetc(ctx.file);

        if( c == EOF )
        {
            new_token.type = TokenType::NO_TOKEN;
            break;
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
            }
        }
        else if( is_white_space(c) )
        {
            if(buf_size == 0)
            {
                first_is_white_space = true;
                ctx.loc.column++;
                break;
            }
            else
            {
                ungetc(c, ctx.file);
                buf[buf_size] = '\0';
            }
        }
        else if( c == '\n' )
        {
            if(buf_size == 0 || first_is_white_space)
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
            }
        }
        else
        {
            if(first_is_white_space)
            {
                ungetc(c, ctx.file);
                new_token.type = TokenType::WHITE_SPACE;
                break;
            }
            else
            {
                buf[buf_size++] = c;
                ctx.loc.column++;
            }
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
            default:
            {
                 
            } break;
        }
    }

    return Token{TokenType::NO_TOKEN};
}

Token peek_token(LexerContext& ctx)
{
    return Token{TokenType::NO_TOKEN};
}

void release_lexer(LexerContext& ctx)
{
    fclose(ctx.file);
}
