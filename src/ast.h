#ifndef AST_H
#define AST_H

enum AstType
{
    EXPRESSION,
    CONSTANT,
    OPERATION,
};

struct AstNode
{
    AstType type;
    AstNode* next; 
};



#endif
