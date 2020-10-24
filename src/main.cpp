#include <stdio.h>

#include "lexer.h"

int main(int argc, char** argv)
{
    LexerContext lexer;
    if(!create_lexer(&lexer, "test_programs/test.ogge"))
    {
        printf("Could not open file.\n");
        return 0;
    }


    //for(Token t : tokens)
    //    print_token(t);

    return 0;
}
