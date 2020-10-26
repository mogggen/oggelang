#include <stdio.h>

#include "block_alloc.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"

int main(int argc, char** argv)
{
    LexerContext lexer;
    if(!create_lexer(&lexer, "test_programs/new.ogge"))
    {
        printf("Could not open file.\n");
        return 0;
    }

    printf("Tokens: \n");

    //while(peek_token(lexer).type != TokenType::NO_TOKEN)
        //fetch_token(lexer);

    BlockAlloc alloc = create_block_alloc(1024);

    AstStatement* root = parse(lexer, alloc);
        
    if( num_error > 0 )
        printf("Compilation failed with %d errors.", num_error);
    else
    {
        printf("\nAST:\n");
        print_statement(root);
    }

    dealloc(alloc);

    
    return 0;
}
