#include "interpreter.h"

#include <vector>

void report_runtime_errro(const char* msg)
{
    printf("RUNTIME ERROR: %s\n", msg);
}

struct Stack
{
    int sp;
    int* stack;
};

inline void push(Stack& stack, int value)
{
    stack.stack[++stack.sp] = value;
}
inline int peek(Stack& stack)
{
    return stack.stack[stack.sp];
}
inline int pop(Stack& stack)
{
    return stack.stack[stack.sp--];
}

#define AT(addr) (addr >= code.size) ? heap[addr-code.size] : mem[addr]

#define SET(addr, value)\
    if(addr >= code.size)\
        heap[addr-code.size] = value;\
    else\
        mem[addr] = value;\


void run(ByteCode code)
{
    int* mem = code.data;
    int pc = 0;

    std::vector<int> heap;

    Stack stack;
    stack.sp = -1;
    stack.stack = (int*)malloc(sizeof(int)*1024);

    while(pc < code.code_size)
    {
         switch((OpCode)code.data[pc++])
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
                    int a = pop(stack);
                    int b = pop(stack);
                    switch((CmpTypes)mem[pc++])
                    {
                        case CmpTypes::EQUAL:         push(stack, a == b ? 1 : 0 ); break;
                        case CmpTypes::NOT_EQUAL:     push(stack, a != b ? 1 : 0 ); break;
                        case CmpTypes::LESS_EQUAL:    push(stack, a <= b ? 1 : 0 ); break;
                        case CmpTypes::GREATER_EQUAL: push(stack, a >= b ? 1 : 0 ); break;
                        case CmpTypes::LESS:          push(stack, a <  b ? 1 : 0 ); break;
                        case CmpTypes::GREATER:       push(stack, a >  b ? 1 : 0 ); break;
                        default: report_runtime_errro("Compare error."); return;
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
        }
    }
}
