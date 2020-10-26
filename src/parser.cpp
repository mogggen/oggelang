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

AstStatement* parse(LexerContext& lexer, BlockAlloc& alloc)
{
    AstStatement* root = nullptr;
    AstStatement* current;

    while(peek_token(lexer).type != TokenType::NO_TOKEN)
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
                        GotoStatement* node = (GotoStatement*)allocate(alloc, sizeof(GotoStatement));
                        node->head.type = StatementType::GOTO;
                        node->head.next = nullptr;
                        node->filename = lexer.loc.filename;
                        node->line_number = t.value;
                        new_statement = (AstStatement*)node;
                    }
                    else
                    {
                        report_error("Expected a line number here.", t.loc);
                        break;
                    }

                } break;
            case TokenType::IF:
            case TokenType::VAR:
            case TokenType::PRINT:
            case TokenType::PRINTC:
            case TokenType::IDENTIFIER:
                {
                    Token t = fetch_non_white_space(lexer);

                    if(t.type == TokenType::ASSIGN)
                    {
                        AstExpression* expression = parse_expression(lexer, alloc);
                        if(expression != nullptr)
                        {
                            AssignStatement* stmt = (AssignStatement*)allocate(alloc, sizeof(AssignStatement));
                            stmt->head.type = StatementType::ASSIGN;
                            stmt->head.next = nullptr;
                            stmt->var_name = token.data;
                            stmt->expression = expression;
                            new_statement = (AstStatement*)stmt;
                        }
                        else
                            break;
                    }
                    else
                    {
                        report_error("Expected assignment statement.", t.loc);
                        break;
                    }
                } break;
            default:
                break;
        }
    

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
