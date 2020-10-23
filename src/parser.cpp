#include "parser.h"
#include <stdio.h>
#include <string.h>


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
const char* VAR_STRING = "var";
const char* MOVE_STRING = "move";
const char* MOVER_STRING = "movr";
const char* REF_STRING = "ref";
const char* DERF_STRING = "derf";
const char* ALLOC_STRING = "alloc";
const char* PRINT_STRING = "print";
const char* PRINTC_STRING = "printc";

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
        case TokenType::ADD:           printf("%s\n", ADD_STRING);                    break;
        case TokenType::SUB:           printf("%s\n", SUB_STRING);                    break;
        case TokenType::MULT:          printf("%s\n", MULT_STRING);                   break;
        case TokenType::DIV:           printf("%s\n", DIV_STRING);                    break;
        case TokenType::LSHIFT:        printf("%s\n", LSHIFT_STRING);                 break;
        case TokenType::RSHIFT:        printf("%s\n", RSHIFT_STRING);                 break;
        case TokenType::AND:           printf("%s\n", AND_STRING);                    break;
        case TokenType::OR:            printf("%s\n", OR_STRING);                     break;
        case TokenType::XOR:           printf("%s\n", XOR_STRING);                    break;
        case TokenType::NOT:           printf("%s\n", NOT_STRING);                    break;
        case TokenType::CMP:           printf("%s\n", CMP_STRING);                    break;
        case TokenType::GOTO:          printf("%s\n", GOTO_STRING);                   break;
        case TokenType::IFGOTO:        printf("%s\n", IFGOTO_STRING);                 break;
        case TokenType::MOVE:          printf("%s\n", MOVE_STRING);                   break;
        case TokenType::MOVER:         printf("%s\n", MOVER_STRING);                  break;
        case TokenType::REF:           printf("%s\n", REF_STRING);                    break;
        case TokenType::DERF:          printf("%s\n", DERF_STRING);                   break;
        case TokenType::ALLOC:         printf("%s\n", ALLOC_STRING);                  break;
        case TokenType::PRINT:         printf("%s\n", PRINT_STRING);                  break;
        case TokenType::PRINTC:        printf("%s\n", PRINTC_STRING);                 break;
        case TokenType::VAR:           printf("%s\n", VAR_STRING);                    break;
        case TokenType::IDENTIFIER:    printf("IDENTIFIER %s\n", t.data);             break;
        case TokenType::ARRAY_BEGIN:   printf("ARRAY_BEGIN\n");                       break;
        case TokenType::ARRAY_END:     printf("ARRAY_END\n");                         break;
        case TokenType::VALUE:         printf("VALUE %d\n", t.value);                 break;
        case TokenType::PROGRAM_LOCATION: printf("PROPERTY\n");                       break;
        case TokenType::IFCONDITION:   {printf("IFCONDITION "); print_if_condition(t.value);}break;
    }
}

int find(const char* str, char c)
{
    int i=0;
    while(str[i] != '\0') 
    {
        if(str[i] == c)
            return i;
        i++;
    }

    return -1;
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
            if      (strcmp(buff, VAR_STRING) == 0)
                tokens.push_back(Token{TokenType::VAR,0,fr.loc});
            else if (strcmp(buff, ADD_STRING) == 0)
                tokens.push_back(Token{TokenType::ADD,0,fr.loc});
            else if (strcmp(buff, SUB_STRING) == 0)
                tokens.push_back(Token{TokenType::SUB,0,fr.loc});
            else if (strcmp(buff, MULT_STRING) == 0)
                tokens.push_back(Token{TokenType::MULT,0,fr.loc});
            else if (strcmp(buff, DIV_STRING) == 0)
                tokens.push_back(Token{TokenType::DIV,0,fr.loc});
            else if (strcmp(buff, LSHIFT_STRING) == 0)
                tokens.push_back(Token{TokenType::LSHIFT,0,fr.loc});
            else if (strcmp(buff, RSHIFT_STRING) == 0)
                tokens.push_back(Token{TokenType::RSHIFT,0,fr.loc});
            else if (strcmp(buff, AND_STRING) == 0)
                tokens.push_back(Token{TokenType::AND, 0,fr.loc});
            else if (strcmp(buff, OR_STRING) == 0)
                tokens.push_back(Token{TokenType::OR,0,fr.loc});
            else if (strcmp(buff, XOR_STRING) == 0)
                tokens.push_back(Token{TokenType::XOR,0,fr.loc});
            else if (strcmp(buff, NOT_STRING) == 0)
                tokens.push_back(Token{TokenType::NOT,0,fr.loc});
            else if (strcmp(buff, CMP_STRING) == 0)
                tokens.push_back(Token{TokenType::CMP,0,fr.loc});
            else if (strcmp(buff, GOTO_STRING) == 0)
                tokens.push_back(Token{TokenType::GOTO,0,fr.loc});
            else if (strcmp(buff, IFGOTO_STRING) == 0)
                tokens.push_back(Token{TokenType::IFGOTO,0,fr.loc});
            else if (strcmp(buff, MOVE_STRING) == 0)
                tokens.push_back(Token{TokenType::MOVE,0,fr.loc});
            else if (strcmp(buff, MOVER_STRING) == 0)
                tokens.push_back(Token{TokenType::MOVER,0,fr.loc});
            else if (strcmp(buff, REF_STRING) == 0)
                tokens.push_back(Token{TokenType::REF,0,fr.loc});
            else if (strcmp(buff, DERF_STRING) == 0)
                tokens.push_back(Token{TokenType::DERF,0,fr.loc});
            else if (strcmp(buff, ALLOC_STRING) == 0)
                tokens.push_back(Token{TokenType::ALLOC,0,fr.loc});
            else if (strcmp(buff, PRINT_STRING) == 0)
                tokens.push_back(Token{TokenType::PRINT,0,fr.loc});
            else if (strcmp(buff, PRINTC_STRING) == 0)
                tokens.push_back(Token{TokenType::PRINTC,0,fr.loc});
            
            else if (strcmp(buff, EQUAL_STRING) == 0)
                tokens.push_back(Token{TokenType::IFCONDITION, EQUAL, fr.loc});
            else if (strcmp(buff, NEQUAL_STRING) == 0)
                tokens.push_back(Token{TokenType::IFCONDITION, NEQUAL, fr.loc});
            else if (strcmp(buff, LEQUAL_STRING) == 0)
                tokens.push_back(Token{TokenType::IFCONDITION, LEQUAL, fr.loc});
            else if (strcmp(buff, GEQUAL_STRING) == 0)
                tokens.push_back(Token{TokenType::IFCONDITION, GEQUAL, fr.loc});
            else if (strcmp(buff, LESS_STRING) == 0)
                tokens.push_back(Token{TokenType::IFCONDITION, LESS, fr.loc});
            else if (strcmp(buff, GREAT_STRING) == 0)
                tokens.push_back(Token{TokenType::IFCONDITION, GREAT, fr.loc});

            else if (*fr.current == '|')
            {
                tokens.push_back(Token{TokenType::ARRAY_BEGIN, 0, fr.loc});
                lexing_state = ARRAY;
                read_char(fr);
                continue;
            }
            else if (*fr.current == '"')
            {
                tokens.push_back(Token{TokenType::ARRAY_BEGIN, 0, fr.loc});
                lexing_state = CHAR_ARRAY;
                read_char(fr);
                continue;
            }
            else if(read_value(fr, &v))
            {
                tokens.push_back(Token{TokenType::VALUE, v, loc});
            }
            else if((v = find(buff, ':')) != -1)
            {
                int line;
                if (sscanf(buff+v+1, "%d", &line) > 0)
                {
                    tokens.push_back(Token{TokenType::PROGRAM_LOCATION, line, fr.loc});
                }
                else
                    printf("this isn't a valid location.\n");
            }
            else // Identifier
            {
                Token t;
                t.type = TokenType::IDENTIFIER;
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
                tokens.push_back(Token{TokenType::ARRAY_END, 0, loc});
                lexing_state = NORMAL;
                read_to_next_word(fr);
            }
            else if(read_value(fr, &v))
            {
                tokens.push_back(Token{TokenType::VALUE, v, loc});
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
                tokens.push_back(Token{TokenType::ARRAY_END, 0, fr.loc});
                lexing_state = NORMAL;
                read_to_next_word(fr);
            }
            else
            {
                FileLocation loc = fr.loc;
                char c = read_char(fr);
                tokens.push_back(Token{TokenType::VALUE, c, loc});
            }
        }

    }

    return tokens;
}
