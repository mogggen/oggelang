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
inline int PEEK(Stack& stack)
{
    return stack.stack[stack.sp];
}
inline int POP(Stack& stack)
{
    return stack.stack[stack.sp--];
}

void run(ByteCode code)
{
    int* mem = code.data;
    int pc = 0;

    std::vector<int> heap;

    Stack stack;
    stack.sp = 0;
    stack.stack = (int*)malloc(sizeof(int)*1024);

    bool flag_equal = false;
    bool flag_greater = false;

    while(pc < code.code_size)
    {
         switch((OpCode)code.data[pc++])
        {
            case OpCode::PUSHC: 
                {
                } break;
            case OpCode::PUSHV:
                {
                } break;
            case OpCode::POP:
                {
                } break;

            case OpCode::ADD: 
                {
                } break;
            case OpCode::SUB:
                {
                } break;
            case OpCode::MULT:
                {
                } break;
            case OpCode::DIV:
                {
                } break;
            case OpCode::AND:
                {
                } break;
            case OpCode::OR:
                {
                } break;
            case OpCode::XOR:
                {
                } break;
            case OpCode::CMP:
                {
                } break;
            case OpCode::NOT:
                {
                } break;
            case OpCode::LSHIFT:
                {
                } break;
            case OpCode::RSHIFT:
                {
                } break;



            case OpCode::REF:
                {
                } break;
            case OpCode::DERF:
                {
                } break;
            case OpCode::MOVE:
                {
                } break;
            case OpCode::MOVED:
                {
                } break;
            case OpCode::ALLOC:
                {
                } break;


            case OpCode::GOTO:
                {
                } break;

            case OpCode::IF:
                {
                } break;
            case OpCode::PRINT:
                {
                } break;
            case OpCode::PRINTC:
                {
                } break;
        }
    }
}
