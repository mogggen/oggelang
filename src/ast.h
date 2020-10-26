#ifndef AST_H
#define AST_H

enum class ExpressionType
{
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

    REF,
    DERF,
    VARIABLE,

    ALLOC,
    CONSTANT,
};

struct AstExpression
{
    ExpressionType type;
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
    PRINTC,
    ASSIGN,
};

struct AstStatement
{
    StatementType type;
    AstStatement* next; 
};

struct GotoStatement
{
    AstStatement head;
    const char* filename;
    int line_number;
};

struct IfStatement
{
    AstStatement head;
    AstExpression* condition;
    AstStatement*  true_statement;
};

struct DeclarationStatement
{
    AstStatement head;
    const char* var_name;
};

struct PrintStatement
{
    AstStatement* head;  
    AstExpression* expression;
    bool print_as_char;
};

struct AssignStatement
{
    AstStatement head;
    const char* var_name;
    AstExpression* expression;
};


void print_statement(AstStatement* stmt, int indent = 0);
void print_expression(AstExpression* expr, int indent = 0);

#endif
