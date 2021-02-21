#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "compiler.h"
#include "interpreter.h"
#include "file_util.h"
#include "debugger.h"

struct CliParameters
{
    bool print_ast = false;
    bool print_opcodes = false;
    bool run_program = true;
    bool gui = false;
    char* filename = nullptr;
};

CliParameters parse_cliargs(int argc, char** argv)
{
    CliParameters parameters;

    for(int i = 1; i < argc; i++)
    {
        if(*(argv+i)[0] == '-')
        {
            if(strcmp(*(argv+i), "-print-ast") == 0)
                parameters.print_ast = true;
            else if(strcmp(*(argv+i), "-print-opcodes") == 0)
                parameters.print_opcodes = true;
            else if(strcmp(*(argv+i), "-no-run") == 0)
                parameters.run_program = false;
            else if(strcmp(*(argv+i), "-gui") == 0)
                parameters.gui = true;
            else
            {
                printf("%s Is invalid flag.\n", *(argv+i));
            }
        }
        else
            parameters.filename = *(argv+i);
    }

    return parameters;
}


int main(int argc, char** argv)
{
    CliParameters parameters = parse_cliargs(argc, argv);
    //parameters.print_ast = true;
    //parameters.print_opcodes = true;
    //parameters.run_program = false;
    //parameters.gui = true;

    if(parameters.gui)
    {
        printf("The gui is currently not available.\n");
        //gui_main();
    }
    else
    {

        if(parameters.filename == nullptr)
        {
            printf("No filename specified.\n");
            return 0;
        }

#ifdef WIN32
        convert_to_unix_file_path(parameters.filename);
#endif

        ByteCode code;
        DebugInfo info;
        if(!compile_program(&code, parameters.filename, parameters.print_ast, &info))
        {
            printf("Compilation Failed\n");
            return 1;
        }
            
        if(parameters.print_opcodes)
        {
            print_opcodes(code);
            printf("------------\n");
        }

        DebugState state;
        start_debug(&state, &code, &info);

        printf("Running %s\n", parameters.filename);
        run(&state);

        if(parameters.run_program)
            run(code);

        free(code.data);
    }

    return 0;
}
