#include "compiler.h"
#include <unordered_map>
#include <algorithm>
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
    std::vector<AddrLinenum> program_line_num;
    int n_variables = 0;
    std::unordered_map<unsigned long, int> var_table;
    std::vector<int> var_addr;
    std::vector<AddrLinenum> goto_addr;
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

bool compile_expression(CompileCtx& cc, AstExpression* expr)
{
    switch(expr->type)
    {
        case ExpressionType::EQUAL:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::CMP);
                cc.program_data.push_back((int)CmpTypes::EQUAL);
            } break;
        case ExpressionType::NEQUAL:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::CMP);
                cc.program_data.push_back((int)CmpTypes::NOT_EQUAL);
            } break;
        case ExpressionType::LEQUAL:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::CMP);
                cc.program_data.push_back((int)CmpTypes::LESS_EQUAL);
            } break;
        case ExpressionType::GEQUAL:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::CMP);
                cc.program_data.push_back((int)CmpTypes::GREATER_EQUAL);
            } break;
        case ExpressionType::LESS:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::CMP);
                cc.program_data.push_back((int)CmpTypes::LESS);
            } break;
        case ExpressionType::GREATER:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::CMP);
                cc.program_data.push_back((int)CmpTypes::GREATER);
            } break;

        case ExpressionType::ADD:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::ADD);
            } break;
        case ExpressionType::SUB:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::SUB);
            } break;
        case ExpressionType::MULT:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::MULT);
            } break;
        case ExpressionType::DIV:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::DIV);
            } break;

        case ExpressionType::AND:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::AND);
            } break;
        case ExpressionType::OR:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::OR);
            } break;
        case ExpressionType::XOR:
            {
                compile_expression(cc, expr->param_a);
                compile_expression(cc, expr->param_b);
                cc.program_data.push_back((int)OpCode::XOR);
            } break;


        case ExpressionType::NOT:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::NOT);
            } break;
        case ExpressionType::LSHIFT:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::LSHIFT);
            } break;
        case ExpressionType::RSHIFT:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::RSHIFT);
            } break;
        case ExpressionType::NEGATE:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::PUSHC);
                cc.program_data.push_back(-1);
                cc.program_data.push_back((int)OpCode::MULT);
            } break;
        case ExpressionType::DECREASE:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::PUSHC);
                cc.program_data.push_back(1);
                cc.program_data.push_back((int)OpCode::SUB);
            } break;
        case ExpressionType::INCREASE:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::PUSHC);
                cc.program_data.push_back(1);
                cc.program_data.push_back((int)OpCode::ADD);
            } break;
        case ExpressionType::DERF:
            {
                compile_expression(cc, expr->param_a);
                cc.program_data.push_back((int)OpCode::DERF);
            } break;
        case ExpressionType::REF:
            {
                int addr = get_variable(cc.var_table, expr->var_name, expr->loc);
                if(addr >= 0)
                {
                    cc.program_data.push_back((int)OpCode::PUSHC);
                    cc.program_data.push_back(addr);
                    cc.var_addr.push_back(cc.program_data.size()-1);
                }
            } break;
        case ExpressionType::VARIABLE:
            {
                int addr = get_variable(cc.var_table, expr->var_name, expr->loc);
                if(addr >= 0)
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

int compile_statement(CompileCtx& cc, AstStatement* stmt)
{
    int prev_size = cc.program_data.size();

    switch(stmt->type)
    {
        case StatementType::GOTO:
            {
                cc.program_line_num.push_back({cc.program_data.size(), stmt->loc.line});
                cc.program_data.push_back((int)OpCode::GOTO);
                cc.goto_addr.push_back({cc.program_data.size(), stmt->goto_line_number});
                cc.program_data.push_back(0);
            } break;
        case StatementType::IF:
            {
                unsigned long beginning_addr = cc.program_data.size();
                if(compile_expression(cc, stmt->expression))
                {
                    cc.program_line_num.push_back({beginning_addr, stmt->loc.line});
                    cc.program_data.push_back((int)OpCode::IF);
                    int location = cc.program_data.size();
                    cc.program_data.push_back(0);
                    int size = compile_statement(cc, stmt->true_statement);
                    cc.program_data[location] = size;
                }
            } break;
        case StatementType::DECLARE:
            {
                int addr = cc.n_variables;
                cc.n_variables++;

                if(!add_variable(cc.var_table, stmt->var_name, addr, stmt->loc))
                    break;
                
                unsigned long beginning_addr = cc.program_data.size();
                if(!compile_expression(cc, stmt->expression))
                    break;

                cc.program_line_num.push_back({beginning_addr, stmt->loc.line});
                cc.program_data.push_back((int)OpCode::MOVED);
                cc.program_data.push_back(addr);
                cc.var_addr.push_back(cc.program_data.size()-1);
            } break;
        case StatementType::ASSIGN:
            {
                unsigned long beginning_addr = cc.program_data.size();
                if(!compile_expression(cc, stmt->expression))
                    break;

                int addr = get_variable(cc.var_table, stmt->var_name, stmt->loc);
                if(addr > 0)
                {
                    cc.program_line_num.push_back({beginning_addr, stmt->loc.line});
                    cc.program_data.push_back((int)OpCode::MOVED);
                    cc.program_data.push_back(addr);
                    cc.var_addr.push_back(cc.program_data.size()-1);
                }
            } break;
        case StatementType::DERF_ASSIGN:
            {
                unsigned long beginning_addr = cc.program_data.size();
                if(!compile_expression(cc, stmt->addr_expression))
                    break;

                if(!compile_expression(cc, stmt->expression))
                    break;

                cc.program_line_num.push_back({beginning_addr, stmt->loc.line});
                cc.program_data.push_back((int)OpCode::MOVE);
            } break;
        case StatementType::PRINT:
            {
                unsigned long beginning_addr = cc.program_data.size();
                if(compile_expression(cc, stmt->expression))
                {
                    cc.program_line_num.push_back({beginning_addr, stmt->loc.line});
                    if(stmt->print_as_char)
                        cc.program_data.push_back((int)OpCode::PRINTC);
                    else
                        cc.program_data.push_back((int)OpCode::PRINT);
                }
            } break;
    }

    return cc.program_data.size() - prev_size;
}

ByteCode compile(AstStatement* root)
{
    CompileCtx cc; 

    AstStatement* statement = root;
    while(statement != nullptr)
    {
        compile_statement(cc, statement);
        statement = statement->next;
    }

    for(auto g : cc.goto_addr)
    {
        auto it = cc.program_line_num.end();
        while(it != cc.program_line_num.begin()) // can use a binary search here.
        {
            --it;
            if(g.line_num <= it->line_num)
                cc.program_data[g.addr] = it->addr;
        }
    }

    for(auto a : cc.var_addr)
        cc.program_data[a] += cc.program_data.size();

    // print compile result
    //printf("\nprogram data:\n");
    //int j = 0;
    //for(int i = 0; i < cc.program_data.size() ; i++)
    //{
    //    printf("%d", cc.program_data[i]);
    //    if( j < cc.program_line_num.size() && i == cc.program_line_num[j].addr )
    //        printf(" %d\n", cc.program_line_num[j++].line_num);
    //    else
    //    {
    //        bool found = false;
    //        for(auto s : cc.goto_addr)
    //        {
    //            if(s.addr == i)
    //            {
    //                printf(" goto %d\n", s.line_num);
    //                found = true;
    //                break;
    //            }
    //        }
    //        if(!found)
    //            printf("\n");
    //    }
    //}

    
    printf("\n%d variables\n", cc.n_variables);
    

    int size = cc.program_data.size() + cc.n_variables;
    int* data = new int[size];
    for(int i=0; i<cc.program_data.size(); i++)
        data[i] = cc.program_data[i];
    
    for(int i=0; i<cc.n_variables; i++)
        data[i + cc.program_data.size()] = 0;

    return ByteCode{size, (int)cc.program_data.size(), data};
}
