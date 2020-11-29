#ifndef STACK_H
#define STACK_H 

#include <stdlib.h>

struct Stack
{
    int sp;
    int* stack;
};

inline void create_stack(Stack* stack, int size)
{
    stack->sp = -1;
    stack->stack = (int*)malloc(sizeof(int)*size);
}

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

#endif /* STACK_H */
