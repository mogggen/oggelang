#include "compiler.h"
#include <unordered_map>
#include "error.h"

constexpr unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

struct CompileCtx
{
    std::vector<int> program_data;
    std::vector<int> static_data;
    std::unordered_map<unsigned long, int> var_table;
    std::vector<int> var_addr;
};

int get_variable(std::unordered_map<unsigned long, int>& var_table, const char* name, FileLocation loc)
{
    auto search = var_table.find(hash_djb2(name));

    if(search == var_table.end())
    {
        report_error("Variable not declared.", loc);
        return -1;
    }
    else
        return search->second;
}

bool add_variable(std::unordered_map<unsigned long, int>& var_table, const char* name, int addr, FileLocation loc)
{
    auto res = var_table.insert({hash_djb2(name), addr});
    if(!res.second)
    {
        report_error("Variable already declared.", loc);
        return false;
    }
    else
        return true;
}

int compile_expression(CompileCtx& cc, AstExpression* expr)
{
    switch(expr->type)
    {
        case ExpressionType::VARIABLE:
            {
                int addr = get_variable(cc.var_table, expr->var_name, expr->loc);
                if(addr > 0)
                {
                    cc.program_data.push_back((int)OpCode::PUSHV);
                    cc.program_data.push_back(addr);
                    cc.var_addr.push_back(cc.program_data.size()-1);
                }
                    
            } break;
        case ExpressionType::CONSTANT:
            {
                cc.program_data.push_back((int)OpCode::PUSHC);
                cc.program_data.push_back(expr->value);
            } break;
        case ExpressionType::ALLOC:
            {
                cc.program_data.push_back((int)OpCode::ALLOC);
            } break;
        default:
            return false;
    }

    return true;
}

bool eval_expression(CompileCtx& cc, AstExpression* expr, int* v)
{
    switch(expr->type)
    {
        case ExpressionType::VARIABLE:
            {
                int addr = get_variable(cc.var_table, expr->var_name, expr->loc);
                if( addr > 0 )
                {
                    *v = cc.static_data[addr];
                    return true;
                }
            } break;
        case ExpressionType::CONSTANT:
            {
                *v = expr->value;
                return true;
            }
        case ExpressionType::ALLOC:
        default:
            return false;
    }
    return false;
}

int compile_statement(CompileCtx& cc, AstStatement* stmt)
{
    int prev_size = cc.program_data.size();

    switch(stmt->type)
    {
        case StatementType::GOTO:
            {

            } break;
        case StatementType::IF:
            {
                if(compile_expression(cc, stmt->expression))
                {
                    cc.program_data.push_back((int)OpCode::IF);
                    int location = cc.program_data.size();
                    cc.program_data.push_back(0);
                    int size = compile_statement(cc, stmt->true_statement);
                    cc.program_data[location] = size;
                }
            } break;
        case StatementType::DECLARE:
            {
                int addr = cc.static_data.size();
                if(!add_variable(cc.var_table, stmt->var_name, addr, stmt->loc))
                    break;
                
                int v;
                if( eval_expression(cc, stmt->expression, &v) )
                {
                    cc.static_data.push_back(v);
                }
            } break;
        case StatementType::PRINT:
            {
                if(compile_expression(cc, stmt->expression))
                {
                    if(stmt->print_as_char)
                        cc.program_data.push_back((int)OpCode::PRINTC);
                    else
                        cc.program_data.push_back((int)OpCode::PRINT);
                }
            } break;
        case StatementType::ASSIGN:
            {
                if(!compile_expression(cc, stmt->expression))
                    break;

                int addr = get_variable(cc.var_table, stmt->var_name, stmt->loc);
                if(addr > 0)
                {
                    cc.program_data.push_back((int)OpCode::MOVED);
                    cc.program_data.push_back(addr);
                    cc.var_addr.push_back(cc.program_data.size()-1);
                }
            } break;
    }

    return cc.program_data.size() - prev_size;
}

void compile(AstStatement* root)
{
    CompileCtx cc; 

    AstStatement* statement = root;
    while(statement != nullptr)
    {
        compile_statement(cc, statement);
        statement = statement->next;
    }

    printf("\nprogram data:\n");
    for(int i : cc.program_data)
        printf("%d\n", i);
    
    printf("\nstatic data:\n");
    for(int i : cc.static_data)
        printf("%d\n", i);
}
