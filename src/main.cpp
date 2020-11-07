#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <queue>

#include "block_alloc.h"
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "linker.h"
#include "interpreter.h"
#include "error.h"

struct Settings
{
    bool print_ast = false;
    bool print_opcodes = false;
    bool run_program = true;
    const char* filename = nullptr;
};

Settings parse_cliargs(int argc, char** argv)
{
    Settings settings;

    for(int i = 1; i < argc; i++)
    {
        if(*(argv+i)[0] == '-')
        {
            if(strcmp(*(argv+i), "-print-ast") == 0)
                settings.print_ast = true;
            else if(strcmp(*(argv+i), "-print-opcodes") == 0)
                settings.print_opcodes = true;
            else if(strcmp(*(argv+i), "-no-run") == 0)
                settings.run_program = false;
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

int find_last_char(const char* str, char c)
{
    int last_pos = -1;
    int i = 0;
    while(str[i] != '\0')
    {
        if(str[i] == c)
            last_pos = i;
        i++;
    }

    return last_pos;
}

int compile_program(const char* filename, std::vector<CompiledObj>* compiled_objects, const Settings& settings )
{

    std::unordered_set<unsigned long> compiled_files;
    std::queue<const char*> compile_queue;

    char filename_buffer[1024];
    int last_slash = find_last_char(filename, '/');
    int directory_size;
    if(last_slash < 0)
    {
        directory_size = 0;
        compile_queue.push(filename);
    }
    else
    {
        directory_size = last_slash+1;
        strncpy(filename_buffer, settings.filename, directory_size);
        compile_queue.push(filename+directory_size);
    }

    printf("directory: ");
    for(int i = 0; i < directory_size; i++)
        printf("%c", filename_buffer[i]);
    printf("\n");


    while(compile_queue.size() > 0)
    {
        const char* filename = compile_queue.front();
        strcpy(filename_buffer+directory_size, filename);

        LexerContext lexer;
        if(!create_lexer(&lexer, filename, filename_buffer))
        {
            printf("Could not open file: %s\n", filename_buffer);
            return 0;
        }
        printf("Compiling : %s\n", filename_buffer);

        BlockAlloc alloc = create_block_alloc(1024);

        AstStatement* root = parse(lexer, alloc);
            
        if( get_num_error() > 0 )
        {
            printf("\nCompilation failed with %d errors.\n", get_num_error());
            return 0;
        }
        else if(settings.print_ast)
            print_statement(root);

        compiled_objects->push_back(compile(root));
        CompiledObj& obj = compiled_objects->back();

        dealloc(alloc);

        if( get_num_error() > 0 )
        {
            printf("\nCompilation failed with %d errors.\n", get_num_error());
            return 0;
        }

        compile_queue.pop();
        compiled_files.insert(obj.filename_hash);

        for(auto file : obj.dependent_files)
        {
            printf("file: %s\n", file);
            auto search = compiled_files.find(hash_djb2(file));
            if(search == compiled_files.end())
            {
                compile_queue.push(file);
            }
        }
    }

    return 1;
}

int main(int argc, char** argv)
{
    Settings settings = parse_cliargs(argc, argv);
    settings.print_opcodes = true;
    settings.run_program = false;

    if(settings.filename == nullptr)
    {
        printf("No filename specified.\n");
        return 0;
    }

    std::vector<CompiledObj> co;
    if(!compile_program(settings.filename, &co, settings))
        return 0;

    ByteCode code;
    if(!link(co, &code))
        return 0;
        
    if(settings.print_opcodes)
    {
        print_opcodes(code);
        printf("------------\n");
    }

    if(settings.run_program)
        run(code);

    delete code.data;

    return 0;
}
