#include "compiler.h"

#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <queue>

#include "block_alloc.h"
#include "lexer.h"
#include "parser.h"
#include "gen_bytecode.h"
#include "linker.h"
#include "error.h"

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

int compile_program(ByteCode* out_code, const char* filename, bool print_ast)
{
    std::vector<CompiledObj> compiled_objects;

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
        strncpy(filename_buffer, filename, directory_size);
        compile_queue.push(filename+directory_size);
    }


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
        else if(print_ast)
            print_statement(root);

        compiled_objects.push_back(gen_bytecode(root));
        CompiledObj& obj = compiled_objects.back();

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
            auto search = compiled_files.find(hash_djb2(file));
            if(search == compiled_files.end())
            {
                compile_queue.push(file);
            }
        }
    }
    

    if(!link(compiled_objects, out_code))
        return 0;

    return 1;
}
