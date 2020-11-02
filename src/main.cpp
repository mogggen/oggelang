#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block_alloc.h"
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "interpreter.h"
#include "error.h"

struct Settings
{
    bool print_ast = false;
    bool print_opcodes = false;
    const char* filename = nullptr;
};

Settings parse_cliargs(int argc, char** argv)
{
    Settings settings;

    for(int i = 1; i < argc; i++)
    {
        if(*(argv+i)[0] == '-')
        {
            if(strcmp(*(argv+i), "-print_ast") == 0)
                settings.print_ast = true;
            else if(strcmp(*(argv+i), "-print_opcodes") == 0)
                settings.print_opcodes = true;
            else
            {
                printf("%s Is invalid flag.\n", *(argv+i));
            }
        }
        else
            settings.filename = *(argv+i);
    }

    return settings;
}

int compile(const char* filename, ByteCode* code)
{
    LexerContext lexer;
    if(!create_lexer(&lexer, filename))
    {
        printf("Could not open file: %s\n", filename);
        return 0;
    }

    BlockAlloc alloc = create_block_alloc(1024);

    AstStatement* root = parse(lexer, alloc);
        
    if( get_num_error() > 0 )
    {
        printf("\nCompilation failed with %d errors.\n", get_num_error());
        return 0;
    }

    *code = compile(root);

    dealloc(alloc);

    if( get_num_error() > 0 )
    {
        printf("\nCompilation failed with %d errors.\n", get_num_error());
        delete code->data;
        return 0;
    }

    return 1;
}

int main(int argc, char** argv)
{
    Settings settings = parse_cliargs(argc, argv);

    if(settings.filename == nullptr)
    {
        printf("No filename specified.\n");
        return 0;
    }

    ByteCode code;
    if(!compile(settings.filename, &code))
        return 0;

    run(code);
    delete code.data;

    return 0;
}
