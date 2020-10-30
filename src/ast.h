#ifndef AST_H
#define AST_H

#include "file_location.h"

enum class ExpressionType
{
    EQUAL,
    NEQUAL,
    LEQUAL,
    GEQUAL,
    LESS,
    GREATER,

    ADD,
    SUB,
    MULT,
    DIV,

    AND,
    OR,
    XOR,
     
    NOT,
    LSHIFT,
    RSHIFT,
    NEGATE,
    DECREASE,
    INCREASE,

    REF,
    DERF,
    VARIABLE,

    ALLOC,
    CONSTANT,
};

struct AstExpression
{
    ExpressionType type;
    FileLocation loc;
    union
    {
        struct
        {
            AstExpression* param_a;
            AstExpression* param_b;
        };
        const char* var_name;
        int value;
    };
};

enum class StatementType
{
    GOTO,
    IF,
    DECLARE,
    PRINT,
    ASSIGN,
    DERF_ASSIGN,
};

struct AstStatement
{
    StatementType type;
    AstStatement* next; 
    FileLocation loc;
    union
    {
        struct // used by GOTO
        {
            const char* goto_filename;
            int goto_line_number;
        };
        struct
        {
            AstExpression* expression; // useb by DECLARE, ASSIGN, DERF_ASSIGN, IF and PRINT
            union
            {
                const char* var_name;           // used by DECLARE and ASSIGN
                AstStatement*  true_statement;  // used by IF
                bool print_as_char;             // used by PRINT
                AstExpression* addr_expression;  // used by DERF_ASSIGN
            };
        };
    };
};

void print_statement(AstStatement* stmt, int indent = 0);
void print_expression(AstExpression* expr, int indent = 0);

#endif
