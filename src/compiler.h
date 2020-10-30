#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include "opcodes.h"
#include <vector>
    
struct AddrLinenum
{
    unsigned long long addr;
    int line_num;
};

struct CompiledObj
{
    std::vector<int> code;
    int nr_variables;
    std::vector<int> var_addr;
    std::vector<AddrLinenum> program_line_num;
    std::vector<AddrLinenum> goto_addr;
};

ByteCode compile(AstStatement* root);

#endif
