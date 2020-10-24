#include "interpreter.h"
#include "parser.h"

void report_runtime_errro(const char* msg)
{
    printf("RUNTIME ERROR: %s\n", msg);
}

void run(ByteCode code)
{
    int* mem = code.data;
    int pc = 0;

    std::vector<int> heap;

    bool flag_equal = false;
    bool flag_greater = false;

    while(pc < code.code_size)
    {
         switch((OpCode)code.data[pc++])
        {
            case OpCode::ADD: 
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] += v;
                    pc += 2;
                } break;
            case OpCode::SUB:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] -= v;
                    pc += 2;
                } break;
            case OpCode::MULT:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] *= v;
                    pc += 2;
                } break;
            case OpCode::DIV:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] /= v;
                    pc += 2;
                } break;
            case OpCode::AND:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] &= v;
                    pc += 2;
                } break;
            case OpCode::OR:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] |= v;
                    pc += 2;
                } break;
            case OpCode::XOR:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] ^= v;
                    pc += 2;
                } break;
            case OpCode::CMP:
                {
                    int v = mem[mem[pc+1]];
                    flag_equal = mem[mem[pc]] == v;
                    flag_greater = mem[mem[pc]] > v;
                    pc += 2;
                } break;
            case OpCode::MOVE:
                {
                    int v = mem[mem[pc+1]];
                    mem[mem[pc]] = v;
                    pc += 2;
                } break;


            case OpCode::ADDC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] += v;
                    pc += 2;
                } break;
            case OpCode::SUBC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] -= v;
                    pc += 2;
                } break;
            case OpCode::MULTC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] *= v;
                    pc += 2;
                } break;
            case OpCode::DIVC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] /= v;
                    pc += 2;
                } break;
            case OpCode::ANDC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] &= v;
                    pc += 2;
                } break;
            case OpCode::ORC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] |= v;
                    pc += 2;
                } break;
            case OpCode::XORC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] ^= v;
                    pc += 2;
                } break;
            case OpCode::CMPC:
                {
                    int v = mem[pc+1];
                    flag_equal = mem[mem[pc]] == v;
                    flag_greater = mem[mem[pc]] > v;
                    pc += 2;
                } break;
            case OpCode::MOVEC:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] = v;
                    pc += 2;
                } break;
            case OpCode::MOVER:
                {
                    int v = mem[mem[pc+1]];
                    int a = mem[mem[pc]];
                    if(a < code.size)
                        mem[a] = v;
                    else if((a-=code.size) < heap.size())
                        heap[a] = v;
                    else
                    {
                        report_runtime_errro("Tried to access invalid memory");
                        return;
                    }
                    pc += 2;
                } break;
            case OpCode::MOVERC:
                {
                    int v = mem[pc+1];
                    int a = mem[mem[pc]];
                    if(a < code.size)
                        mem[a] = v;
                    else if((a-=code.size) < heap.size())
                        heap[a] = v;
                    else
                    {
                        report_runtime_errro("Tried to access invalid memory");
                        return;
                    }
                    pc += 2;
                } break;


            case OpCode::REF:
                {
                    int v = mem[pc+1];
                    mem[mem[pc]] = v;
                    pc += 2;
                } break;
            case OpCode::DERF:
                {
                    int v = mem[mem[pc+1]];
                    if(v < code.size)
                        mem[mem[pc]] = mem[v];
                    else if((v-=code.size) < heap.size())
                        mem[mem[pc]] = heap[v];
                    else
                    {
                        report_runtime_errro("Tried to access invalid memory");
                        return;
                    }
                    pc += 2;
                } break;


            case OpCode::NOT:
                {
                    mem[mem[pc]] = ~mem[mem[pc]];
                    pc += 1;
                } break;
            case OpCode::LSHIFT:
                {
                    mem[mem[pc++]] <<= 1;
                } break;
            case OpCode::RSHIFT:
                {
                    mem[mem[pc++]] >>= 1;
                } break;

            case OpCode::GOTO:
                {
                    pc = mem[pc+1];
                } break;

            case OpCode::IFGOTO:
                {
                    pc += 2;
                    switch(mem[pc-2])
                    {
                        case EQUAL:  if(flag_equal) pc = mem[pc-1]; break;
                        case NEQUAL: if(!flag_equal) pc = mem[pc-1]; break;
                        case LEQUAL: if(!flag_greater) pc = mem[pc-1]; break;
                        case GEQUAL: if(flag_equal || flag_greater) pc = mem[pc-1]; break;
                        case LESS:   if(!flag_equal && !flag_greater) pc = mem[pc-1]; break;
                        case GREAT:  if(flag_greater) pc = mem[pc-1]; break;
                    }
                } break;

            case OpCode::ALLOC:
                {
                    heap.push_back(0);
                    heap.push_back(0);
                    mem[mem[pc]] = heap.size()-2 + code.size;
                    pc += 1;
                } break;

            case OpCode::PRINT:
                {
                    printf("%d", mem[mem[pc++]]);
                } break;
            case OpCode::PRINTC:
                {
                    printf("%c", (char)mem[mem[pc++]]);
                } break;
        }
    }
}
