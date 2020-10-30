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
        case ExpressionType::EQUAL:
            {
                printf("EQUAL\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::NEQUAL:
            {
                printf("NEQUAL\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::LEQUAL:
            {
                printf("LEQUAL\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::GEQUAL:
            {
                printf("GEQUAL\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::LESS:
            {
                printf("LESS\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
        case ExpressionType::GREATER:
            {
                printf("GREATER\n");
                print_expression(expr->param_a, indent+2);
                print_expression(expr->param_b, indent+2);
            } break;
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
                print_expression(expr->param_a, indent+2);
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
                printf("GOTO %s : %d\n", stmt->goto_filename, stmt->goto_line_number);
            } break;
        case StatementType::IF: 
            {
                printf("IF\n");
                print_expression(stmt->expression, indent+2);
                print_statement(stmt->true_statement, indent+2);
            } break;
        case StatementType::DECLARE: 
            {
                printf("DECLARE %s\n", stmt->var_name);
                print_expression(stmt->expression, indent+2);
            } break;
        case StatementType::PRINT: 
            {
                if(stmt->print_as_char)
                    printf("PRINT as char\n");
                else
                    printf("PRINT as int\n");
                print_expression(stmt->expression, indent+2);
            } break;
        case StatementType::ASSIGN: 
            {
                printf("ASSIGN %s\n", stmt->var_name);
                print_expression(stmt->expression, indent+2);
            } break;
        case StatementType::DERF_ASSIGN: 
            {
                printf("DERF_ASSIGN %s\n", stmt->var_name);
                print_expression(stmt->addr_expresson, indent+2);
                print_expression(stmt->expression, indent+2);
            } break;
    }

    print_statement(stmt->next);
}
