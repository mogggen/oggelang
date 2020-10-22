#include <stdio.h>

#include "file_reader.h"
#include "parser.h"
#include "compiler.h"
#include "interpreter.h"

int main(int argc, char** argv)
{
    FileReader reader;
    if(!create_file_reader(&reader, "test_programs/test.ogge"))
    {
        printf("Could not open file.\n");
        return 0;
    }

    auto tokens = tokenize(reader);
    close(reader);
    
    //for(Token t : tokens)
    //    print_token(t);

    ByteCode code = compile(tokens);

    for(int i=0; i<code.size; i++)
        printf("%d\n", code.data[i]);

    printf("_______\n");

    run(code);
    
    free(code.data);

    return 0;
}
