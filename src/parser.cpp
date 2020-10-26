#include "parser.h"

#include "error.h"


AstExpression* parse_expression(LexerContext& lexer, BlockAlloc& alloc)
{
    Token t = fetch_non_white_space(lexer);
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
        default:
            { 
                report_error("Expected an expression.", t.loc);
                return nullptr;
            } break;
    }

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
                Token t = fetch_non_white_space(lexer);

                const char* filename;
                
                if(t.type == TokenType::COLON)
                    filename = lexer.loc.filename;
                else if(t.type == TokenType::IDENTIFIER)
                {
                    filename = t.data;
                    t = fetch_non_white_space(lexer);

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

                t = fetch_non_white_space(lexer);

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
                t = fetch_non_white_space(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::IF:
            {
                AstExpression* expression = parse_expression(lexer, alloc);
                if(expression != nullptr)
                {
                    if(peek_token(lexer).type == TokenType::WHITE_SPACE)
                        fetch_token(lexer);

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
                Token t = fetch_non_white_space(lexer);
                
                if(t.type == TokenType::IDENTIFIER)
                {
                    const char* var_name = t.data;
                    t = fetch_non_white_space(lexer);

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

                t = fetch_non_white_space(lexer);
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
                Token t = fetch_non_white_space(lexer);
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
                Token t = fetch_non_white_space(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
            } break;
        case TokenType::IDENTIFIER:
            {
                Token t = fetch_non_white_space(lexer);

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
                
                t = fetch_non_white_space(lexer);
                if(t.type != TokenType::NEW_LINE)
                    report_error("Unexpected symbol.", t.loc);
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
