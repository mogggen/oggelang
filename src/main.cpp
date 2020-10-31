#include <stdio.h>
#include <stdlib.h>

#include "block_alloc.h"
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "interpreter.h"
#include "error.h"

struct Settings
{
    bool print_ast_tree = false;
    bool print_opcodes = false;
    const char* filename = nullptr;
};

Settings parse_cliargs(int argc, char** argv)
{
    Settings settings;

    for(int i = 1; i < argc; i++)
    {
        char* str = argv[i];
        if(strcmp(str, "-print_ast"))
            settings.print_ast_tree = true;
        else if(strcmp(str, "-print_ast"))
            settings.print_opcodes = true;
        else
            settings.filename = str;
    }

    return settings;
}

int main(int argc, char** argv)
{

    LexerContext lexer;
    if(!create_lexer(&lexer, "test_programs/new.ogge"))
    {
        printf("Could not open file.\n");
        return 0;
    }

    //printf("Tokens: \n");

    BlockAlloc alloc = create_block_alloc(1024);

    AstStatement* root = parse(lexer, alloc);
        
    if( num_error > 0 )
        printf("Compilation failed with %d errors.", num_error);
    else if(true)
    {
        printf("AST:\n");
        print_statement(root);
    }

    ByteCode code = compile(root);

    dealloc(alloc);

    //printf("\nbyte code:\n");
    //for(int i = 0; i < code.size; i++)
    //    printf("%d\n", code.data[i]);


    if(true)
    {
        print_opcodes(code);
        printf("__________________\n");
    }

    run(code);

    delete code.data;
    
    return 0;
}
