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
#include "util.h"

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

int compile_program(ByteCode* out_code, const char* filename, bool print_ast, DebugInfo* out_dbginfo)
{
    std::vector<CompiledObj> compiled_objects;

    BlockAlloc symbol_names_alloc = create_block_alloc(1024);

    std::unordered_set<unsigned long> compiled_files; // set of all compiled files
    std::queue<const char*> compile_queue;

    char filepath_buffer[2048]; // buffer for directory and filename

    // find directory
    int last_slash = find_last_char(filename, '\\');
    int directory_size;
    if(last_slash < 0)
    {
        directory_size = 0;
        compile_queue.push(filename);
    }
    else
    {
        directory_size = last_slash+1;
        strncpy(filepath_buffer, filename, directory_size); // copy directory to filepath_buffer
        compile_queue.push(filename+directory_size); // push only filename and not entire path
    }


    while(compile_queue.size() > 0)
    {
        const char* filename = compile_queue.front();
        strcpy(filepath_buffer+directory_size, filename); // append filename to directory

        // create lexer
        LexerContext lexer;
        if(!create_lexer(&lexer, filepath_buffer, filepath_buffer, &symbol_names_alloc))
        {
            printf("Could not open file: %s\n", filepath_buffer);
            return 0;
        }
        printf("Compiling : %s\n", filepath_buffer);

        BlockAlloc alloc = create_block_alloc(1024);


        // parser
        AstStatement* root = parse(lexer, alloc);
            
        if( get_num_error() > 0 )
        {
            printf("\nCompilation failed with %d errors.\n", get_num_error());
            return 0;
        }
        else if(print_ast)
            print_statement(root);

        // generate byte code
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

        // add dependent files to compile queue
        for(auto file : obj.dependent_files)
        {
            strcpy(filepath_buffer+directory_size, file); // append filename to directory
            auto search = compiled_files.find(hash_djb2(filepath_buffer));
            if(search == compiled_files.end())
            {
                compile_queue.push(file);
            }
        }
    }
    

    if(out_dbginfo == nullptr)
    {
        if(!link(compiled_objects, out_code))
            return 0;
        dealloc(symbol_names_alloc);
    }
    else
    {
        dealloc(out_dbginfo->symbol_names_alloc);

        if(!link_debug(compiled_objects, out_code, out_dbginfo))
            return 0;
        out_dbginfo->symbol_names_alloc = symbol_names_alloc;
    }


    return 1;
}
