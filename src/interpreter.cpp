#include "interpreter.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
        }
    }
}

void print_opcodes(ByteCode code)
{
    int pc = 0;

    printf("\nNr varaiables: %d\nVariables start at: %d\n\n", code.size -code.code_size, code.code_size );

    int digits = 0;
    int v = code.code_size;
    while(v > 0)
    {
        digits++;
        v /= 10;
    }

    while(pc < code.code_size)
    {
        printf("%*d | ", digits, pc);
        switch((OpCode)code.data[pc++])
        {
            case OpCode::PUSHC:     printf("PUSHC %d\n", code.data[pc++]); break;
            case OpCode::PUSHV:     printf("PUSHV %d\n", code.data[pc++]); break;
            case OpCode::POP:       printf("POP\n"); break;
            case OpCode::ADD:       printf("ADD\n"); break;
            case OpCode::SUB:       printf("SUB\n"); break;
            case OpCode::MULT:      printf("MULT\n"); break;
            case OpCode::DIV:       printf("DIV\n"); break;
            case OpCode::AND:       printf("AND\n"); break;
            case OpCode::OR:        printf("OR\n"); break;
            case OpCode::XOR:       printf("XOR\n"); break;
            case OpCode::CMP:       
                {
                    printf("CMP ");
                    switch((CmpTypes)code.data[pc++])
                    {
                        case CmpTypes::EQUAL: printf("EQUAL\n"); break;
                        case CmpTypes::NOT_EQUAL: printf("NOT_EQUAL\n"); break;
                        case CmpTypes::LESS_EQUAL: printf("LESS_EQUAL\n"); break;
                        case CmpTypes::GREATER_EQUAL: printf("GREATER_EQUAL\n"); break;
                        case CmpTypes::LESS: printf("LESS\n"); break;
                        case CmpTypes::GREATER: printf("GREATER\n"); break;
                    }
                } break;
            case OpCode::NOT:       printf("NOT\n"); break;
            case OpCode::LSHIFT:    printf("LSHIFT\n"); break;
            case OpCode::RSHIFT:    printf("RSHIFT\n"); break;
            case OpCode::REF:       printf("REF\n"); break;
            case OpCode::DERF:      printf("DERF\n"); break;
            case OpCode::MOVE:      printf("MOVE\n"); break;
            case OpCode::MOVED:     printf("MOVED %d\n", code.data[pc++]); break;
            case OpCode::ALLOC:     printf("ALLOC\n"); break;
            case OpCode::GOTO:      printf("GOTO %d\n", code.data[pc++]); break;
            case OpCode::IF:        printf("IF %d\n", code.data[pc++]); break;
            case OpCode::PRINT:     printf("PRINT\n"); break;
            case OpCode::PRINTC:    printf("PRINTC\n"); break;
        }
    }
}
