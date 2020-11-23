#include "gen_bytecode.h"
#include <unordered_map>
#include <algorithm>
#include "error.h"
#include "util.h"

struct Variable
{
    int addr;
    const char* name;
};

struct GeneratorCtx
{
    std::vector<int> program_data;
    std::vector<int> program_line_num;
    int n_variables = 0;
    std::unordered_map<unsigned long, Variable> var_table;
    std::vector<int> var_addr;
    std::vector<AddrLinenum> goto_addr;
    unsigned long filename_hash;
    std::vector<const char*> dependent_files;
};

int get_variable(std::unordered_map<unsigned long, Variable>& var_table, const char* name, FileLocation loc)
{
    auto search = var_table.find(hash_djb2(name));

    if(search == var_table.end())
    {
        report_error("Variable not declared.", loc);
        return -1;
    }
    else
        return search->second.addr;
}

bool add_variable(std::unordered_map<unsigned long, Variable>& var_table, const char* name, int addr, FileLocation loc)
{
    auto res = var_table.insert({hash_djb2(name), Variable{addr, name}});
    if(!res.second)
    {
        report_error("Variable already declared.", loc);
        return false;
    }
    else
        return true;
}

bool gen_bytecode_expression(GeneratorCtx& gc, AstExpression* expr)
{
    switch(expr->type)
    {
        case ExpressionType::EQUAL:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::CMP);
                gc.program_data.push_back((int)CmpTypes::EQUAL);
            } break;
        case ExpressionType::NEQUAL:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::CMP);
                gc.program_data.push_back((int)CmpTypes::NOT_EQUAL);
            } break;
        case ExpressionType::LEQUAL:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::CMP);
                gc.program_data.push_back((int)CmpTypes::LESS_EQUAL);
            } break;
        case ExpressionType::GEQUAL:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::CMP);
                gc.program_data.push_back((int)CmpTypes::GREATER_EQUAL);
            } break;
        case ExpressionType::LESS:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::CMP);
                gc.program_data.push_back((int)CmpTypes::LESS);
            } break;
        case ExpressionType::GREATER:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::CMP);
                gc.program_data.push_back((int)CmpTypes::GREATER);
            } break;

        case ExpressionType::ADD:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::ADD);
            } break;
        case ExpressionType::SUB:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::SUB);
            } break;
        case ExpressionType::MULT:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::MULT);
            } break;
        case ExpressionType::DIV:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::DIV);
            } break;

        case ExpressionType::AND:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::AND);
            } break;
        case ExpressionType::OR:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::OR);
            } break;
        case ExpressionType::XOR:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gen_bytecode_expression(gc, expr->param_b);
                gc.program_data.push_back((int)OpCode::XOR);
            } break;


        case ExpressionType::NOT:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::NOT);
            } break;
        case ExpressionType::LSHIFT:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::LSHIFT);
            } break;
        case ExpressionType::RSHIFT:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::RSHIFT);
            } break;
        case ExpressionType::NEGATE:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::PUSHC);
                gc.program_data.push_back(-1);
                gc.program_data.push_back((int)OpCode::MULT);
            } break;
        case ExpressionType::DECREASE:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::PUSHC);
                gc.program_data.push_back(1);
                gc.program_data.push_back((int)OpCode::SUB);
            } break;
        case ExpressionType::INCREASE:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::PUSHC);
                gc.program_data.push_back(1);
                gc.program_data.push_back((int)OpCode::ADD);
            } break;
        case ExpressionType::DERF:
            {
                gen_bytecode_expression(gc, expr->param_a);
                gc.program_data.push_back((int)OpCode::DERF);
            } break;
        case ExpressionType::REF:
            {
                int addr = get_variable(gc.var_table, expr->var_name, expr->loc);
                if(addr >= 0)
                {
                    gc.program_data.push_back((int)OpCode::PUSHC);
                    gc.program_data.push_back(addr);
                    gc.var_addr.push_back(gc.program_data.size()-1);
                }
            } break;
        case ExpressionType::VARIABLE:
            {
                int addr = get_variable(gc.var_table, expr->var_name, expr->loc);
                if(addr >= 0)
                {
                    gc.program_data.push_back((int)OpCode::PUSHV);
                    gc.program_data.push_back(addr);
                    gc.var_addr.push_back(gc.program_data.size()-1);
                }
            } break;
        case ExpressionType::CONSTANT:
            {
                gc.program_data.push_back((int)OpCode::PUSHC);
                gc.program_data.push_back(expr->value);
            } break;
        case ExpressionType::ALLOC:
            {
                gc.program_data.push_back((int)OpCode::ALLOC);
            } break;
        case ExpressionType::LIST_ELEMENT:
            {

                gc.program_data.push_back((int)OpCode::ALLOC);
                gc.program_data.push_back((int)OpCode::COPY);
                gen_bytecode_expression(gc, expr->list_element);
                gc.program_data.push_back((int)OpCode::MOVE);
                gc.program_data.push_back((int)OpCode::COPY);
                
                AstExpression* list_element = expr->next_element;

                while(list_element != nullptr)
                {
                    gc.program_data.push_back((int)OpCode::PUSHC);
                    gc.program_data.push_back(1);
                    gc.program_data.push_back((int)OpCode::COPY);
                    gc.program_data.push_back((int)OpCode::ALLOC);
                    gc.program_data.push_back((int)OpCode::MOVE);
                    gc.program_data.push_back((int)OpCode::DERF);
                    gc.program_data.push_back((int)OpCode::COPY);
                    gen_bytecode_expression(gc, list_element->list_element);
                    gc.program_data.push_back((int)OpCode::MOVE);

                    list_element = list_element->next_element;
                }

            } break;
        default:
            return false;
    }

    return true;
}

int gen_bytecode_statement(GeneratorCtx& gc, AstStatement* stmt)
{
    int prev_size = gc.program_data.size();

    unsigned long beginning_addr = gc.program_data.size();

    switch(stmt->type)
    {
        case StatementType::GOTO:
            {
                gc.program_data.push_back((int)OpCode::GOTO);
                if(stmt->goto_filename == nullptr)
                {
                    gc.goto_addr.push_back({gc.program_data.size(), gc.filename_hash, stmt->goto_line_number});
                }
                else
                {
                    gc.goto_addr.push_back({gc.program_data.size(), hash_djb2(stmt->goto_filename), stmt->goto_line_number});
                    gc.dependent_files.push_back(stmt->goto_filename);
                }
                //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
                gc.program_data.push_back(0);
            } break;
        case StatementType::IF:
            {
                if(gen_bytecode_expression(gc, stmt->expression))
                {
                    gc.program_data.push_back((int)OpCode::IF);
                    //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
                    int location = gc.program_data.size();
                    gc.program_data.push_back(0);

                    int size = gen_bytecode_statement(gc, stmt->true_statement);
                    gc.program_data[location] = size;
                }
            } break;
        case StatementType::DECLARE:
            {
                int addr = gc.n_variables;
                gc.n_variables++;

                if(!add_variable(gc.var_table, stmt->var_name, addr, stmt->loc))
                    break;
                
                if(!gen_bytecode_expression(gc, stmt->expression))
                    break;

                gc.program_data.push_back((int)OpCode::MOVED);
                gc.program_data.push_back(addr);
                gc.var_addr.push_back(gc.program_data.size()-1);
                //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
            } break;
        case StatementType::ASSIGN:
            {
                if(!gen_bytecode_expression(gc, stmt->expression))
                    break;

                int addr = get_variable(gc.var_table, stmt->var_name, stmt->loc);
                if(addr > 0)
                {
                    gc.program_data.push_back((int)OpCode::MOVED);
                    gc.program_data.push_back(addr);
                    gc.var_addr.push_back(gc.program_data.size()-1);
                    //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
                }
            } break;
        case StatementType::DERF_ASSIGN:
            {
                if(!gen_bytecode_expression(gc, stmt->addr_expression))
                    break;

                if(!gen_bytecode_expression(gc, stmt->expression))
                    break;

                gc.program_data.push_back((int)OpCode::MOVE);
                //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
            } break;
        case StatementType::PRINT:
            {
                if(gen_bytecode_expression(gc, stmt->expression))
                {
                    if(stmt->print_as_char)
                        gc.program_data.push_back((int)OpCode::PRINTC);
                    else
                        gc.program_data.push_back((int)OpCode::PRINT);
                    //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
                }
            } break;
        case StatementType::SCAN:
            {
                if(gen_bytecode_expression(gc, stmt->expression))
                {
                    gc.program_data.push_back((int)OpCode::SCAN);
                    //gc.program_line_num.push_back({beginning_addr, gc.filename_hash, stmt->loc.line});
                }
            } break;
    }

    return gc.program_data.size() - prev_size;
}

CompiledObj gen_bytecode(AstStatement* root)
{
    GeneratorCtx gc; 
    gc.filename_hash = hash_djb2(root->loc.filename);

    AstStatement* statement = root;
    while(statement != nullptr)
    {
        int stmt_size = gen_bytecode_statement(gc, statement);
        for(int i=0; i<stmt_size;i++)
            gc.program_line_num.push_back(statement->loc.line);
        statement = statement->next;
    }

    gc.program_data.push_back((int)OpCode::END);
    gc.program_line_num.push_back(gc.program_line_num.back()); // linenumber for END

    std::vector<const char*> var_names(gc.n_variables);

    for(auto var : gc.var_table)
        var_names[var.second.addr] = var.second.name;


    return CompiledObj{
        gc.program_data,
        gc.n_variables,
        gc.var_addr,
        var_names,
        gc.program_line_num,
        gc.goto_addr,
        root->loc.filename,
        gc.filename_hash,
        gc.dependent_files
    };
}
