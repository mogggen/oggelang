#include "parser.h"

#include "error.h"

AstExpression* parse_expression(LexerContext& lexer, BlockAlloc& alloc);

bool is_double_arg_expression(AstExpression* e)
{
    return  e->type != ExpressionType::NOT &&
            e->type != ExpressionType::LSHIFT &&
            e->type != ExpressionType::RSHIFT &&
            e->type != ExpressionType::NEGATE &&
            e->type != ExpressionType::DECREASE &&
            e->type != ExpressionType::INCREASE &&
            e->type != ExpressionType::REF &&
            e->type != ExpressionType::DERF &&
            e->type != ExpressionType::VARIABLE &&
            e->type != ExpressionType::ALLOC && 
            e->type != ExpressionType::CONSTANT;
}

AstExpression* balance_expression(AstExpression* expr)
{
    if(is_double_arg_expression(expr))
    {
        AstExpression* right_expr = expr->param_b;

        if( expr->type > right_expr->type )
        {
            expr->param_b = right_expr->param_a;
            right_expr->param_a = expr;
            expr = right_expr;

            expr->param_a = balance_expression(expr->param_a);
        }
    }

    return expr;
}

AstExpression* parse_single_expression(LexerContext& lexer, BlockAlloc& alloc)
{
    Token t = fetch_token(lexer);
    AstExpression* expr = nullptr; 
    switch(t.type)
    {
        case TokenType::IDENTIFIER:
            {
                expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                expr->type = ExpressionType::VARIABLE;
                expr->var_name = t.data;
            } break;
        case TokenType::CONSTANT:
            {
                expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                expr->type = ExpressionType::CONSTANT;
                expr->value = t.value;
            } break;
        case TokenType::ALLOC:
            {
                expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                expr->type = ExpressionType::ALLOC;
            } break;
        case TokenType::REF:
            {
                Token t = peek_token(lexer);
                if(t.type == TokenType::IDENTIFIER)
                {
                    fetch_token(lexer);
                    expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                    expr->type = ExpressionType::REF;
                    expr->var_name = t.data;
                }
                else
                {
                    report_error("Expected a variable after reference.", t.loc);
                    return nullptr;
                }
            } break;
        case TokenType::DERF:
            {
                AstExpression* e = parse_single_expression(lexer, alloc);
                if(e == nullptr)
                {
                    report_error("Expected expression after dereference.", t.loc);
                    return nullptr;
                }

                if(!is_double_arg_expression(e))
                {
                    expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                    expr->type = ExpressionType::DERF;
                    expr->param_a = e;
                }
                else
                {
                    report_error("Expected a variable here.", e->loc);
                    return nullptr;
                }
            } break;
        case TokenType::NOT:
            {
                AstExpression* e = parse_single_expression(lexer, alloc);
                if(e == nullptr)
                {
                    report_error("Expected expression after '~'.", t.loc);
                    return nullptr;
                }

                if(!is_double_arg_expression(e))
                {
                    expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                    expr->type = ExpressionType::NOT;
                    expr->param_a = e;
                }
                else
                {
                    report_error("Expected variable here.", e->loc);
                    return nullptr;
                }
                
            } break;
        case TokenType::ADD:
            {
                Token t = peek_token(lexer);
                if(t.type == TokenType::ADD)
                {
                    fetch_token(lexer);
                    AstExpression* e = parse_single_expression(lexer, alloc);
                    if(e == nullptr)
                    {
                        report_error("Expected expression after '--'.", t.loc);
                        return nullptr;
                    }
                    
                    if(is_double_arg_expression(e))
                    {
                        report_error("Expected a Variable or constant.", e->loc);
                        return nullptr;
                    }

                    expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                    expr->type = ExpressionType::INCREASE;
                    expr->param_a = e;
                }
                else
                {
                    report_error("Unexpected symbol.", t.loc);
                }
            } break;
        case TokenType::SUB:
            {
                Token t = peek_token(lexer);
                if(t.type == TokenType::SUB)
                {
                    fetch_token(lexer);
                    AstExpression* e = parse_single_expression(lexer, alloc);
                    if(e == nullptr)
                    {
                        report_error("Expected expression after '--'.", t.loc);
                        return nullptr;
                    }
                    
                    if(is_double_arg_expression(e))
                    {
                        report_error("Expected a Variable or constant.", e->loc);
                        return nullptr;
                    }

                    expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                    expr->type = ExpressionType::DECREASE;
                    expr->param_a = e;
                }
                else
                {
                    AstExpression* e = parse_single_expression(lexer, alloc);
                    if(e == nullptr)
                    {
                        report_error("Expected expression after '-'.", t.loc);
                        return nullptr;
                    }
                    
                    if(is_double_arg_expression(e))
                    {
                        report_error("Expected a Variable or constant.", e->loc);
                        return nullptr;
                    }

                    expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                    expr->type = ExpressionType::NEGATE;
                    expr->param_a = e;
                }
            } break;
        case TokenType::LESS:
            {
                Token t = fetch_token(lexer);
                if(t.type == TokenType::LESS)
                {
                    AstExpression* e = parse_single_expression(lexer, alloc);
                    if(e == nullptr)
                    {
                        report_error("Expected expression after '<<'.", t.loc);
                        return nullptr;
                    }

                    if(!is_double_arg_expression(e))
                    {
                        expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                        expr->type = ExpressionType::LSHIFT;
                        expr->param_a = e;
                    }
                    else
                    {
                        report_error("Expected a Variable or constant.", e->loc);
                        return nullptr;
                    }

                }
                else
                {
                    report_error("Unexpected symbol.", t.loc);
                    return nullptr;
                }
            } break;
        case TokenType::GREATER:
            {
                Token t = fetch_token(lexer);
                if(t.type == TokenType::GREATER)
                {
                    AstExpression* e = parse_single_expression(lexer, alloc);
                    if(e == nullptr)
                    {
                        report_error("Expected expression after '>>'", t.loc);
                        return nullptr;
                    }

                    if(!is_double_arg_expression(e))
                    {
                        expr = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                        expr->type = ExpressionType::RSHIFT;
                        expr->param_a = e;
                    }
                    else
                    {
                        report_error("Expected a Variable or constant.", e->loc);
                        return nullptr;
                    }

                }
                else
                {
                    report_error("Unexpected symbol.", t.loc);
                    return nullptr;
                }
            } break;
        default:
            { 
                report_error("Expected an expression.", t.loc);
                return nullptr;
            } break;
    }

    expr->loc = t.loc;
    return expr;
}

AstExpression* parse_operator_expression(LexerContext& lexer, BlockAlloc& alloc, AstExpression* left_expr)
{
    Token t = peek_token(lexer);
    ExpressionType type;
    bool has_fetched = false;

    switch(t.type)
    {
        case TokenType::NEW_LINE: return nullptr;
        case TokenType::ADD: type       = ExpressionType::ADD; break;
        case TokenType::SUB: type       = ExpressionType::SUB; break;
        case TokenType::MULT: type      = ExpressionType::MULT; break;
        case TokenType::DIV: type       = ExpressionType::DIV; break;
        case TokenType::AND: type       = ExpressionType::AND; break;
        case TokenType::OR: type        = ExpressionType::OR; break;
        case TokenType::XOR: type       = ExpressionType::XOR; break;
        case TokenType::LESS:
            {
                fetch_token(lexer);
                has_fetched = true;
                if(peek_token(lexer).type == TokenType::EQUALS)
                {
                    fetch_token(lexer);
                    type = ExpressionType::LEQUAL;
                }
                else
                    type = ExpressionType::LESS;
            } break;
        case TokenType::GREATER:
            {
                fetch_token(lexer);
                has_fetched = true;
                if(peek_token(lexer).type == TokenType::EQUALS)
                {
                    fetch_token(lexer);
                    type = ExpressionType::GEQUAL;
                }
                else
                    type = ExpressionType::GREATER;
            } break;
        case TokenType::EQUALS:
            {
                fetch_token(lexer);
                has_fetched = true;
                if(peek_token(lexer).type == TokenType::EQUALS)
                {
                    fetch_token(lexer);
                    type = ExpressionType::EQUAL;
                }
                else
                {
                    report_error("Unrecognized operation.", peek_token(lexer).loc);
                    return nullptr;
                }
            } break;
        case TokenType::EXCLAMATION:
            {
                fetch_token(lexer);
                has_fetched = true;
                if(peek_token(lexer).type == TokenType::EQUALS)
                {
                    fetch_token(lexer);
                    type = ExpressionType::NEQUAL;
                }
                else
                {
                    report_error("Unrecognized operation.", peek_token(lexer).loc);
                    return nullptr;
                }
            } break;
        default:
            {
                return nullptr;
            }
    }



    if(!has_fetched)
        fetch_token(lexer);

    AstExpression* right_expr = parse_expression(lexer, alloc);
    if(right_expr == nullptr)
    {
        return nullptr;
    }

    AstExpression* oper = (AstExpression*)allocate(alloc, sizeof(AstExpression)); 
    oper->type = type;
    oper->loc = t.loc;
    oper->param_a = left_expr;
    oper->param_b = right_expr;

    oper = balance_expression(oper);


    return oper;
}

AstExpression* parse_expression(LexerContext& lexer, BlockAlloc& alloc)
{
    AstExpression* expr = parse_single_expression(lexer, alloc);
    if(expr == nullptr)
        return nullptr;
    
    AstExpression* rest = parse_operator_expression(lexer, alloc, expr);

    if(rest != nullptr)
        return rest;
    else
        return expr;
}

AstStatement* parse_statement(LexerContext& lexer, BlockAlloc& alloc)
{
    AstStatement* new_statement = nullptr;
    Token token = fetch_token(lexer);

    switch(token.type) 
    {
        case TokenType::GOTO:
            {
                Token t = fetch_token(lexer);

                const char* filename;
                
                if(t.type == TokenType::COLON)
                    filename = lexer.loc.filename;
                else if(t.type == TokenType::IDENTIFIER)
                {
                    filename = t.data;
                    t = fetch_token(lexer);

                    if(t.type != TokenType::COLON)
                    {
                        report_error("Expected a colon after filename.", t.loc);
                        break;
                    }
                }
                else
                {
                    report_error("Expected a file location.", t.loc);
                    break;
                }

                t = fetch_token(lexer);

                if(t.type == TokenType::CONSTANT)
                {
                    new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                    new_statement->type = StatementType::GOTO;
                    new_statement->goto_filename = filename;
                    new_statement->goto_line_number = t.value;
                }
                else
                {
                    report_error("Expected a line number here.", t.loc);
                    break;
                }
                t = fetch_token(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::IF:
            {
                AstExpression* expression = parse_expression(lexer, alloc);
                if(expression != nullptr)
                {
                    AstStatement* statement = parse_statement(lexer, alloc);;
                    if(statement != nullptr)
                    {
                        new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                        new_statement->type = StatementType::IF;
                        new_statement->expression = expression;
                        new_statement->true_statement = statement;
                    }
                    else
                    {
                        report_error("Expected a statement after if.", token.loc);
                        break;
                    }
                }
            } break;
        case TokenType::VAR:
            {
                Token t = fetch_token(lexer);
                
                if(t.type == TokenType::IDENTIFIER)
                {
                    const char* var_name = t.data;
                    t = fetch_token(lexer);

                    if(t.type == TokenType::ASSIGN)
                    {
                        AstExpression* expression = parse_expression(lexer, alloc);
                        if(expression != nullptr)
                        {
                            new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                            new_statement->type = StatementType::DECLARE;
                            new_statement->var_name = var_name;
                            new_statement->expression = expression;
                        }
                        else
                            break;
                    }
                    else if(t.type == TokenType::NEW_LINE)
                    {
                        new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                        new_statement->type = StatementType::DECLARE;
                        new_statement->var_name = var_name;
                        new_statement->expression = (AstExpression*)allocate(alloc, sizeof(AstExpression));
                        new_statement->expression->type = ExpressionType::CONSTANT;
                        new_statement->expression->value = 0;
                        break;
                    }
                    else
                    {
                        report_error("Expected assignment or new line.", t.loc);
                        break;
                    }
                }
                else
                {
                    report_error("Expected an identifier after 'var'.", t.loc);
                }

                t = fetch_token(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::PRINT:
            {
                AstExpression* expression = parse_expression(lexer, alloc);
                if(expression != nullptr)
                {
                    new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                    new_statement->type = StatementType::PRINT;
                    new_statement->expression = expression;
                    new_statement->print_as_char = false;
                }
                else
                    break;
                Token t = fetch_token(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::PRINTC:
            {
                AstExpression* expression = parse_expression(lexer, alloc);
                if(expression != nullptr)
                {
                    new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                    new_statement->type = StatementType::PRINT;
                    new_statement->expression = expression;
                    new_statement->print_as_char = true;
                }
                else
                    break;
                Token t = fetch_token(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::IDENTIFIER:
            {
                Token t = fetch_token(lexer);

                if(t.type == TokenType::ASSIGN)
                {
                    AstExpression* expression = parse_expression(lexer, alloc);
                    if(expression != nullptr)
                    {
                        new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                        new_statement->type = StatementType::ASSIGN;
                        new_statement->var_name = token.data;
                        new_statement->expression = expression;
                    }
                    else
                        break;
                }
                else
                {
                    report_error("Expected assignment statement.", t.loc);
                    break;
                }
                
                t = fetch_token(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::DERF:
            {
                AstExpression* ae = parse_single_expression(lexer, alloc);
                if(ae == nullptr)
                {
                    report_error("Expeted an expression after dereference", token.loc);
                    break;
                }

                if(!is_double_arg_expression(ae))
                {
                    Token t = fetch_token(lexer);
                    if(t.type != TokenType::ASSIGN)
                    {
                        report_error("Expected a '=' here.", t.loc);
                        break;
                    }

                    AstExpression* e = parse_expression(lexer, alloc);
                    if(e == nullptr)
                    {
                        report_error("Expected an expression after this.", t.loc);
                        break;
                    }

                    new_statement = (AstStatement*)allocate(alloc, sizeof(AstStatement));
                    new_statement->type = StatementType::DERF_ASSIGN;
                    new_statement->addr_expression = ae;
                    new_statement->expression = e;
                }
                else
                {
                    report_error("Expected varaiable here.", ae->loc);
                    break;
                }
            } break;
        default:
            break;
    }

    if(new_statement != nullptr)
    {
        new_statement->next = nullptr;
        new_statement->loc = token.loc;
    }
    return new_statement;
}

AstStatement* parse(LexerContext& lexer, BlockAlloc& alloc)
{
    AstStatement* root = nullptr;
    AstStatement* current;

    while(peek_token(lexer).type != TokenType::NO_TOKEN)
    {
        AstStatement* new_statement = parse_statement(lexer, alloc);
        if(new_statement == nullptr)
            continue;

        if (root == nullptr)
        {
            root = new_statement;
            current = new_statement;
        }
        else
        {
            current->next = new_statement;
            current = current->next;
        }
    }

    return root;
}
