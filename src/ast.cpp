#include "ast.h"

#include <stdio.h>

void print_expression(AstExpression* expr, int indent)
{
    if(expr == nullptr)
        return;
    for(int i = 0; i < indent; i++)
        printf(" ");

    switch(expr->type)
    {
        case ExpressionType::ADD:
            {
                printf("ADD\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::SUB:
            {
                printf("SUB\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::MULT:
            {
                printf("MULT\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::DIV:
            {
                printf("DIV\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::AND:
            {
                printf("AND\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::OR:
            {
                printf("OR\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::XOR:
            {
                printf("XOR\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::NOT:
            {
                printf("NOT\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::LSHIFT:
            {
                printf("LSHIFT\n");
                print_expression(expr->param_a, indent+2);
            } break;
        case ExpressionType::RSHIFT:
            {
                printf("RSHIFT\n");
                print_expression(expr->param_a, indent+2);
            } break;
        case ExpressionType::REF:
            {
                printf("REF %s\n", expr->var_name);
            } break;
        case ExpressionType::DERF:
            {
                printf("DERF %s\n", expr->var_name);
            } break;
        case ExpressionType::VARIABLE:
            {
                printf("VARIABLE %s\n", expr->var_name);
            } break;
        case ExpressionType::ALLOC:
            {
                printf("ALLOC\n");
            } break;
        case ExpressionType::CONSTANT:
            {
                printf("CONSTANT %d\n", expr->value);
            } break;
    }
}

void print_statement(AstStatement* stmt, int indent)
{
    if(stmt == nullptr)
        return;
    for(int i = 0; i < indent; i++)
        printf(" ");

    switch(stmt->type)
    {
        case StatementType::GOTO: 
            {
                GotoStatement* s = (GotoStatement*)stmt;
                printf("GOTO %s : %d\n", s->filename, s->line_number);
            } break;
        case StatementType::IF: 
            {
                IfStatement* s = (IfStatement*)stmt;
                printf("IF\n");
                print_expression(s->condition, indent+2);
                print_statement(s->true_statement, indent+2);
            } break;
        case StatementType::DECLARE: 
            {
                printf("DECLARE \n");
            } break;
        case StatementType::PRINT: 
            {
                printf("PRINT\n");
            } break;
        case StatementType::PRINTC: 
            {
                printf("PRINTC\n");
            } break;
        case StatementType::ASSIGN: 
            {
                printf("ASSIGN\n");
                print_expression(((AssignStatement*)stmt)->expression, indent+2);
            } break;
    }
}
