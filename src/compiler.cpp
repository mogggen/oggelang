#include "compiler.h"
#include <unordered_map>

void report_error(const char* err, FileLocation loc)
{
    printf("ERROR: %s Line %d Column %d\n", err, loc.line, loc.column);
}

unsigned long hash_djb2(const char*str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = hash*33 + c;

    return hash;
}

int get_variable(std::unordered_map<unsigned long, int>& var_table, char* name, FileLocation loc)
{
    auto search = var_table.find(hash_djb2(name));

    if(search == var_table.end())
        report_error("Variable not declared.", loc);

    else
        return search->second;

    return -1;
}

void add_variable(std::unordered_map<unsigned long, int>& var_table, char* name, int addr, FileLocation loc)
{
    auto res = var_table.insert({hash_djb2(name), addr});
    if(!res.second)
        report_error("Variable already declared.", loc);
}

void compile_inst_with_constant(int* dest, OpCode var_instr, OpCode const_instr, Token arg1, Token arg2, std::unordered_map<unsigned long, int>& var_table)
{
    if(arg1.type == TokenType::IDENTIFIER)
        dest[1] = get_variable(var_table, arg1.data, arg1.loc);
    else
        report_error("Argument must be a variable.", arg1.loc);

    if(arg2.type == TokenType::IDENTIFIER)
    {
        dest[0] = (int)var_instr;
        dest[2] = get_variable(var_table, arg2.data, arg2.loc);
    }
    else if(arg2.type == TokenType::VALUE)
    {
        dest[0] = (int)const_instr;
        dest[2] = arg2.value;
        
    }
    else
        report_error("Argument must be variable or constant.", arg2.loc);
}

ByteCode compile(std::vector<Token> tokens)
{
    int n_instruction_bytes = 0;
    int n_variables = 0;

    for(Token t : tokens)
    {
        switch(t.type)
        {
            case TokenType::ADD:
            case TokenType::SUB:
            case TokenType::MULT:
            case TokenType::DIV:
            case TokenType::AND:
            case TokenType::OR:
            case TokenType::XOR:
            case TokenType::CMP:
            case TokenType::MOVE:
            case TokenType::MOVER:
            case TokenType::REF:
            case TokenType::DERF:
            case TokenType::IFGOTO: 
                n_instruction_bytes += 3; break;

            case TokenType::NOT:
            case TokenType::LSHIFT:
            case TokenType::RSHIFT:
            case TokenType::GOTO: 
            case TokenType::ALLOC:
            case TokenType::PRINT:
            case TokenType::PRINTC:
                n_instruction_bytes += 2; break;

            case TokenType::VAR: 
                n_variables += 1; break;

            default:
                break;
        }
    }

    int data_size = n_instruction_bytes+n_variables;
    int* program_data = (int*)malloc(sizeof(int)*data_size);

    std::unordered_map<unsigned long, int> var_table;
    std::unordered_map<int, int> location_map; // maps line number to instruction address
    std::vector<std::pair<int, Token>> link_locations; // address to insert address to name

    int instr_addr = 0;
    int var_addr = n_instruction_bytes;


    auto it = tokens.begin();
    while(it < tokens.end())
    {
        Token t = *(it++);
        location_map.insert({t.loc.line, instr_addr});
        switch(t.type)
        {
            case TokenType::ADD: 
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::ADD, OpCode::ADDC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::SUB:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::SUB, OpCode::SUBC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::MULT:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::MULT, OpCode::MULTC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::DIV:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::DIV, OpCode::DIVC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::AND:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::AND, OpCode::ANDC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::OR:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::OR, OpCode::ORC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::XOR:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::XOR, OpCode::XORC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::CMP:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::CMP, OpCode::CMPC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::MOVE:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    compile_inst_with_constant(program_data + instr_addr, OpCode::MOVE, OpCode::MOVEC, arg1, arg2, var_table);
                    instr_addr += 3;
                } break;
            case TokenType::MOVER:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    if(arg1.type == TokenType::IDENTIFIER)
                        program_data[instr_addr+1] = get_variable(var_table, arg1.data, arg1.loc);
                    else
                        report_error("Argument must be a variable.", arg1.loc);

                    if(arg2.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+2] = get_variable(var_table, arg2.data, arg2.loc);
                        program_data[instr_addr] = (int)OpCode::MOVER;
                    }
                    else if(arg2.type == TokenType::VALUE)
                    {
                        program_data[instr_addr+2] = arg2.value;
                        program_data[instr_addr] = (int)OpCode::MOVERC;
                    }
                    else
                        report_error("Argument must be variable.", arg2.loc);
                    instr_addr += 3;
                } break;
            case TokenType::REF:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    program_data[instr_addr] = (int)OpCode::REF;
                    if(arg1.type == TokenType::IDENTIFIER)
                        program_data[instr_addr+1] = get_variable(var_table, arg1.data, arg1.loc);
                    else
                        report_error("Argument must be a variable.", arg1.loc);

                    if(arg2.type == TokenType::IDENTIFIER)
                        program_data[instr_addr+2] = get_variable(var_table, arg2.data, arg2.loc);
                    else
                        report_error("Argument must be variable.", arg2.loc);
                    instr_addr += 3;
                } break;
            case TokenType::DERF:
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    program_data[instr_addr] = (int)OpCode::DERF;
                    if(arg1.type == TokenType::IDENTIFIER)
                        program_data[instr_addr+1] = get_variable(var_table, arg1.data, arg1.loc);
                    else
                        report_error("Argument must be a variable.", arg1.loc);

                    if(arg2.type == TokenType::IDENTIFIER)
                        program_data[instr_addr+2] = get_variable(var_table, arg2.data, arg2.loc);
                    else
                        report_error("Argument must be variable.", arg2.loc);
                    instr_addr += 3;
                } break;
            case TokenType::IFGOTO: 
                {
                    Token arg1 = *(it++);
                    Token arg2 = *(it++);
                    program_data[instr_addr] = (int)OpCode::IFGOTO;

                    if(arg1.type == TokenType::IFCONDITION)
                    {
                        program_data[instr_addr+1] = arg1.value;
                    }
                    else
                        report_error("Following 'if' should be EQUAL, NEQUAL, LEQUAL, GEQUAL, LESS or GREAT.", arg1.loc);

                    if(arg2.type == TokenType::PROGRAM_LOCATION)
                    {
                        link_locations.push_back({instr_addr+2, arg2});
                    }
                    else 
                        report_error("'if' needs a goto location.", arg2.loc);

                    instr_addr += 3;
                } break;

            case TokenType::NOT:
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::NOT;
                    if(arg.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+1] = get_variable(var_table, arg.data, arg.loc);
                    }
                    else
                        report_error("Argument must be a variable.", arg.loc);

                    instr_addr += 2;
                } break;
            case TokenType::LSHIFT:
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::LSHIFT;
                    if(arg.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+1] = get_variable(var_table, arg.data, arg.loc);
                    }
                    else
                        report_error("Argument must be a variable.", arg.loc);

                    instr_addr += 2;
                } break;
            case TokenType::RSHIFT:
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::RSHIFT;
                    if(arg.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+1] = get_variable(var_table, arg.data, arg.loc);
                    }
                    else
                        report_error("Argument must be a variable.", arg.loc);

                    instr_addr += 2;
                } break;
            case TokenType::GOTO: 
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::GOTO;
                    if(arg.type == TokenType::PROGRAM_LOCATION)
                    {
                        link_locations.push_back({instr_addr+1, arg});
                    }
                    else
                        report_error("'goto' needs a goto location.", arg.loc);

                    instr_addr += 2;
                } break;
            case TokenType::ALLOC: 
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::ALLOC;
                    if(arg.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+1] = get_variable(var_table, arg.data, arg.loc);
                    }
                    else
                        report_error("'goto' needs a goto location.", arg.loc);

                    instr_addr += 2;
                } break;
            case TokenType::PRINT:
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::PRINT;
                    if(arg.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+1] = get_variable(var_table, arg.data, arg.loc);
                    }
                    else
                        report_error("Argument must be a variable.", arg.loc);

                    instr_addr += 2;
                } break;
            case TokenType::PRINTC:
                {
                    Token arg = *(it++);
                    program_data[instr_addr] = (int)OpCode::PRINTC;
                    if(arg.type == TokenType::IDENTIFIER)
                    {
                        program_data[instr_addr+1] = get_variable(var_table, arg.data, arg.loc);
                    }
                    else
                        report_error("Argument must be a variable.", arg.loc);

                    instr_addr += 2;
                } break;

            case TokenType::VAR: 
                {
                    Token identifier = *(it++);
                    Token value = *(it++);

                    if (identifier.type == TokenType::IDENTIFIER)
                    {
                        if (value.type == TokenType::VALUE)
                        {
                            add_variable(var_table, identifier.data, var_addr, t.loc);
                            program_data[var_addr++] = value.value;
                        }
                        else
                            report_error("Expected a value here.", value.loc);
                    }
                    else
                        report_error("Expected a variable name.", identifier.loc);

                } break;

            default:
                break;
        }
        
    }


    for(auto p : link_locations)
    {
        int target_addr = p.first;
        Token line = p.second;

        auto addr = location_map.find(line.value);
        if (addr != location_map.end())
        {
            program_data[target_addr] = addr->second;
        }
        else
            report_error("Couldn't find instruction at program position.", line.loc);
    }

    return ByteCode{data_size, n_instruction_bytes, program_data};
}
