#include "debugger.h"

void start_debug(DebugState* dbgstate, ByteCode* code, DebugInfo* dbginfo)
{
    dbgstate->code = code;
    dbgstate->dbginfo = dbginfo;
}

#define AT(addr) (addr >= code_size) ? heap[addr-code_size] : mem[addr]

#define SET(addr, value)\
    if(addr >= code_size)\
        heap[addr-code_size] = value;\
    else\
        mem[addr] = value;\

void step_instruction(DebugState* dbgstate)
{
    int& pc = dbgstate->pc;
    auto& heap = dbgstate->heap; 
    auto& stack = dbgstate->stack;
    int* mem = dbgstate->code->data;
    int code_size = dbgstate->code->code_size;

    switch((OpCode)mem[pc++])
    {
    case OpCode::PUSHC: 
        {
            push(stack, mem[pc++]);
        } break;
    case OpCode::PUSHV:
        {
            int a = mem[pc++];
            push(stack, AT(a));
        } break;
    case OpCode::POP:
        {
            pop(stack);
        } break;
    case OpCode::COPY:
        {
            push(stack, peek(stack));
        } break;

    case OpCode::ADD: 
        {
            int v = pop(stack) + pop(stack);
            push(stack, v);
        } break;
    case OpCode::SUB:
        {
            int v = pop(stack);
            v = pop(stack) - v;
            push(stack, v);
        } break;
    case OpCode::MULT:
        {
            int v = pop(stack) * pop(stack);
            push(stack, v);
        } break;
    case OpCode::DIV:
        {
            int v = pop(stack);
            v = pop(stack) / v;
            push(stack, v);
        } break;
    case OpCode::AND:
        {
            int v = pop(stack) & pop(stack);
            push(stack, v);
        } break;
    case OpCode::OR:
        {
            int v = pop(stack) | pop(stack);
            push(stack, v);
        } break;
    case OpCode::XOR:
        {
            int v = pop(stack) ^ pop(stack);
            push(stack, v);
        } break;
    case OpCode::NOT:
        {
            push(stack, ~pop(stack));
        } break;
    case OpCode::LSHIFT:
        {
            push(stack, pop(stack) << 1);
        } break;
    case OpCode::RSHIFT:
        {
            push(stack, pop(stack) >> 1);
        } break;
    case OpCode::CMP:
        {
            int b = pop(stack);
            int a = pop(stack);
            switch((CmpTypes)mem[pc++])
            {
                case CmpTypes::EQUAL:         push(stack, a == b ? 1 : 0 ); break;
                case CmpTypes::NOT_EQUAL:     push(stack, a != b ? 1 : 0 ); break;
                case CmpTypes::LESS_EQUAL:    push(stack, a <= b ? 1 : 0 ); break;
                case CmpTypes::GREATER_EQUAL: push(stack, a >= b ? 1 : 0 ); break;
                case CmpTypes::LESS:          push(stack, a <  b ? 1 : 0 ); break;
                case CmpTypes::GREATER:       push(stack, a >  b ? 1 : 0 ); break;
                default: report_runtime_errro("Compare error.", pc-2); return;
            }
        } break;


    case OpCode::REF:
        {
            push(stack, mem[pc++]);
        } break;
    case OpCode::DERF:
        {
            int a = pop(stack);
            push(stack, AT(a));
        } break;
    case OpCode::MOVE:
        {
            int v = pop(stack);
            int a = pop(stack);
            SET(a, v);
        } break;
    case OpCode::MOVED:
        {
            int a = mem[pc++];
            SET(a, pop(stack));
        } break;
    case OpCode::ALLOC:
        {
            int a = heap.size();
            heap.push_back(0);
            heap.push_back(0);
            push(stack, a);
        } break;


    case OpCode::GOTO:
        {
            pc = mem[pc];
        } break;

    case OpCode::IF:
        {
            if(pop(stack))
                pc++;
            else
                pc += mem[pc]+1;

        } break;
    case OpCode::PRINT:
        {
            printf("%d", pop(stack));
        } break;
    case OpCode::PRINTC:
        {
            printf("%c", (char)pop(stack));
        } break;
    case OpCode::SCAN:
        {
            int v;
            int a = pop(stack);
            scanf("%d", &v);
            SET(a, v);
        } break;

    case OpCode::END:
        {
            pc = code.code_size; // jump to end
        } break;
    }
}

void step_line(DebugState* dbgstate);

void run(DebugState* dbgstate);
