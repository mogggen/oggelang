#include <stdio.h>

#include "file_reader.h"
#include "parser.h"

int main(int argc, char** argv)
{
    printf("Hello there\n");


    FileReader reader;
    if(!create_file_reader(&reader, "test_programs/test.ogge"))
    {
        printf("File doesn't finns Jävla IDIOT!\n");
        return 0;
    }

    tokenize(reader);

    close(reader);

    return 0;
}
