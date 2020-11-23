#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H 

#include "file_location.h"

struct DebugVar
{
    const char* name;
    int addr;
    FileLocation loc;
};

struct FilenameMapping
{
    int addr;
    const char* filename;
};

struct DebugInfo
{
    int n_variables;
    DebugVar* variables;

    int* line_nums; // same size as code.data

    int n_filenames;
    FilenameMapping* filename_mapping;
};

#endif /* DEBUG_INFO_H */
