#ifndef AST_H
#define AST_H

enum AstType
{
    EXPRESSION,
    CONSTANT,
    OPERATION,
};

enum ExpressionType
{
    ADD,
    SUB,
    MULT,
    DIV,

};

struct AstNode
{
    AstType type;
    AstNode* next; 
};



#endif
