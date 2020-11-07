#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include "opcodes.h"
#include <vector>
    
struct AddrLinenum
{
    unsigned long long addr;
    unsigned long filename_hash;
    int line_num;
};

struct CompiledObj
{
    std::vector<int> code;
    int nr_variables;
    std::vector<int> var_addr;
    std::vector<AddrLinenum> program_line_num;
    std::vector<AddrLinenum> goto_addr;
    unsigned long filename_hash;
    std::vector<const char*> dependent_files;
};

constexpr unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

CompiledObj compile(AstStatement* root);

#endif
