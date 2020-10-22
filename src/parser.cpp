#include "parser.h"
#include <stdio.h>
#include <string.h>

static const int EQUAL   = 0;
static const int NEQUAL  = 1;
static const int LEQUAL  = 2;
static const int GEQUAL  = 3;
static const int LESS    = 4;
static const int GREAT   = 5;


const char* EQUAL_STRING   = "EQUAL";
const char* NEQUAL_STRING  = "NEQUAL";
const char* LEQUAL_STRING  = "LEQUAL";
const char* GEQUAL_STRING  = "GEQUAL";
const char* LESS_STRING    = "LESS";
const char* GREAT_STRING   = "GREAT";

const char* ADD_STRING = "add";
const char* SUB_STRING = "sub";
const char* MULT_STRING = "mult";
const char* DIV_STRING = "div";
const char* LSHIFT_STRING = "lsh";
const char* RSHIFT_STRING = "rsh";
const char* AND_STRING = "and";
const char* OR_STRING = "or";
const char* XOR_STRING = "xor";
const char* NOT_STRING = "not";
const char* CMP_STRING = "cmp";
const char* GOTO_STRING = "goto";
const char* IFGOTO_STRING = "if";
const char* MOVE_STRING = "move";
const char* PRINT_STRING = "print";
const char* PRINTC_STRING = "printc";
const char* SET_STRING = "set";

void print_if_condition(int c)
{
    switch(c)
    {
        case EQUAL:  printf("EQUAL\n");  break;
        case NEQUAL: printf("NEQUAL\n"); break;
        case LEQUAL: printf("LEQUAL\n"); break;
        case GEQUAL: printf("GEQUAL\n"); break;
        case LESS:   printf("LESS\n");   break;
        case GREAT:  printf("GREAT\n");  break;
    }
}

void print_token(Token t)
{
    switch(t.type)
    {
        case ADD:           printf("%s\n", ADD_STRING);                    break;
        case SUB:           printf("%s\n", SUB_STRING);                    break;
        case MULT:          printf("%s\n", MULT_STRING);                   break;
        case DIV:           printf("%s\n", DIV_STRING);                    break;
        case LSHIFT:        printf("%s\n", LSHIFT_STRING);                 break;
        case RSHIFT:        printf("%s\n", RSHIFT_STRING);                 break;
        case AND:           printf("%s\n", AND_STRING);                    break;
        case OR:            printf("%s\n", OR_STRING);                     break;
        case XOR:           printf("%s\n", XOR_STRING);                    break;
        case NOT:           printf("%s\n", NOT_STRING);                    break;
        case CMP:           printf("%s\n", CMP_STRING);                    break;
        case GOTO:          printf("%s\n", GOTO_STRING);                   break;
        case IFGOTO:        printf("%s\n", IFGOTO_STRING);                 break;
        case MOVE:          printf("%s\n", MOVE_STRING);                   break;
        case PRINT:         printf("%s\n", PRINT_STRING);                  break;
        case PRINTC:        printf("%s\n", PRINTC_STRING);                 break;
        case SET:           printf("%s\n", SET_STRING);                    break;
        case IDENTIFIER:    printf("IDENTIFIER %s\n", t.data);             break;
        case ARRAY_BEGIN:   printf("ARRAY_BEGIN\n");                       break;
        case ARRAY_END:     printf("ARRAY_END\n");                         break;
        case VALUE:         printf("VALUE %d\n", t.value);                 break;
        case FILE_LOCATION: printf("FILE_LOCATION\n");                     break;
        case IFCONDITION:   {printf("IFCONDITION "); print_if_condition(t.value);}break;
    }
}

std::vector<Token> tokenize(FileReader& fr)
{
    enum State { NORMAL, ARRAY, CHAR_ARRAY };
    
    std::vector<Token> tokens;

    char buff[64];
    State lexing_state = NORMAL;
    int v;

    while(!fr.is_finished)
    {
        FileLocation loc = fr.loc;
        if(lexing_state == NORMAL)
        {
            peek_word(fr, buff, 64);
            if      (strcmp(buff, SET_STRING) == 0)
                tokens.push_back(Token{SET,0,fr.loc});
            else if (strcmp(buff, ADD_STRING) == 0)
                tokens.push_back(Token{ADD,0,fr.loc});
            else if (strcmp(buff, SUB_STRING) == 0)
                tokens.push_back(Token{SUB,0,fr.loc});
            else if (strcmp(buff, MULT_STRING) == 0)
                tokens.push_back(Token{MULT,0,fr.loc});
            else if (strcmp(buff, DIV_STRING) == 0)
                tokens.push_back(Token{DIV,0,fr.loc});
            else if (strcmp(buff, LSHIFT_STRING) == 0)
                tokens.push_back(Token{LSHIFT,0,fr.loc});
            else if (strcmp(buff, RSHIFT_STRING) == 0)
                tokens.push_back(Token{RSHIFT,0,fr.loc});
            else if (strcmp(buff, AND_STRING) == 0)
                tokens.push_back(Token{AND, 0,fr.loc});
            else if (strcmp(buff, OR_STRING) == 0)
                tokens.push_back(Token{OR,0,fr.loc});
            else if (strcmp(buff, XOR_STRING) == 0)
                tokens.push_back(Token{XOR,0,fr.loc});
            else if (strcmp(buff, NOT_STRING) == 0)
                tokens.push_back(Token{NOT,0,fr.loc});
            else if (strcmp(buff, CMP_STRING) == 0)
                tokens.push_back(Token{CMP,0,fr.loc});
            else if (strcmp(buff, GOTO_STRING) == 0)
                tokens.push_back(Token{GOTO,0,fr.loc});
            else if (strcmp(buff, IFGOTO_STRING) == 0)
                tokens.push_back(Token{IFGOTO,0,fr.loc});
            else if (strcmp(buff, MOVE_STRING) == 0)
                tokens.push_back(Token{MOVE,0,fr.loc});
            else if (strcmp(buff, PRINT_STRING) == 0)
                tokens.push_back(Token{PRINT,0,fr.loc});
            else if (strcmp(buff, PRINTC_STRING) == 0)
                tokens.push_back(Token{PRINTC,0,fr.loc});
            
            else if (strcmp(buff, EQUAL_STRING) == 0)
                tokens.push_back(Token{IFCONDITION, EQUAL, fr.loc});
            else if (strcmp(buff, NEQUAL_STRING) == 0)
                tokens.push_back(Token{IFCONDITION, NEQUAL, fr.loc});
            else if (strcmp(buff, LEQUAL_STRING) == 0)
                tokens.push_back(Token{IFCONDITION, LEQUAL, fr.loc});
            else if (strcmp(buff, GEQUAL_STRING) == 0)
                tokens.push_back(Token{IFCONDITION, GEQUAL, fr.loc});
            else if (strcmp(buff, LESS_STRING) == 0)
                tokens.push_back(Token{IFCONDITION, LESS, fr.loc});
            else if (strcmp(buff, GREAT_STRING) == 0)
                tokens.push_back(Token{IFCONDITION, GREAT, fr.loc});

            else if (*fr.current == '|')
            {
                tokens.push_back(Token{ARRAY_BEGIN, 0, fr.loc});
                lexing_state = ARRAY;
                read_char(fr);
                continue;
            }
            else if (*fr.current == '"')
            {
                tokens.push_back(Token{ARRAY_BEGIN, 0, fr.loc});
                lexing_state = CHAR_ARRAY;
                read_char(fr);
                continue;
            }
            else if(read_value(fr, &v))
            {
                tokens.push_back(Token{VALUE, v, loc});
            }
            else // Identifier
            {
                Token t;
                t.type = IDENTIFIER;
                t.loc = fr.loc;
                int size = strlen(buff)+1;
                t.data = (char*)malloc(size);
                memcpy(t.data, buff, size);
                tokens.push_back(t);
            }

            read_to_next_word(fr);
        }
        else if (lexing_state == ARRAY)
        {
            FileLocation loc = fr.loc;
            if(*fr.current == '|')
            {
                tokens.push_back(Token{ARRAY_END, 0, loc});
                lexing_state = NORMAL;
                read_to_next_word(fr);
            }
            else if(read_value(fr, &v))
            {
                tokens.push_back(Token{VALUE, v, loc});
            }
            else
            {
                printf("Array contained non int value, moron.\n");
            }
        }
        else // lexing_state == CHAR_ARRAY
        {
            if(*fr.current == '"')
            {
                tokens.push_back(Token{ARRAY_END, 0, fr.loc});
                lexing_state = NORMAL;
                read_to_next_word(fr);
            }
            else
            {
                FileLocation loc = fr.loc;
                char c = read_char(fr);
                tokens.push_back(Token{VALUE, c, loc});
            }
        }

    }

    for(Token t : tokens)
        print_token(t);

    return tokens;
}
