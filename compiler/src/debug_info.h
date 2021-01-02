#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H 

#include "file_location.h"
#include "block_alloc.h"

struct DebugVar
{
    const char* name;
    int addr;
    FileLocation loc;
};

struct FilenameMapping
{
    int first_addr;
    int last_addr;
    const char* filename;
};

struct DebugInfo
{
    int n_variables;
    DebugVar* variables;

    int* line_nums; // same size as code.data, maps an address to linenumber

    int n_filenames;
    FilenameMapping* filename_mapping;

    BlockAlloc symbol_names_alloc;
};

void linenumber_from_instruction_addr(DebugInfo* info, int addr, char** out_filename, int* out_linenum);

#endif /* DEBUG_INFO_H */
