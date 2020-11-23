#include "opcodes.h"
#include <stdio.h>

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
            case OpCode::COPY:      printf("COPY\n"); break;
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
            case OpCode::SCAN:    printf("SCAN\n"); break;
            case OpCode::END:       printf("END\n"); break;
        }
    }
}

void print_opcodes(ByteCode code, DebugInfo* dbginfo)
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
        printf("%*d : %*d | ", digits, pc, digits, dbginfo->line_nums[pc]);
        switch((OpCode)code.data[pc++])
        {
            case OpCode::PUSHC:     printf("PUSHC %d\n", code.data[pc++]); break;
            case OpCode::PUSHV:     printf("PUSHV %d\n", code.data[pc++]); break;
            case OpCode::POP:       printf("POP\n"); break;
            case OpCode::COPY:      printf("COPY\n"); break;
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
            case OpCode::SCAN:    printf("SCAN\n"); break;
            case OpCode::END:       printf("END\n"); break;
        }
    }
}
