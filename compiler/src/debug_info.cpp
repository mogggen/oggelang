#include "debug_info.h"

void linenumber_from_instruction_addr(DebugInfo* info, int addr, char** out_filename, int* out_linenum)
{
    // find filename of address
    int middle_idx;

    while(1)
    {
        middle_idx = info->n_filenames/2;
        auto mapping = info->filename_mapping[middle_idx];

        if(addr < mapping.first_addr)
            middle_idx = middle_idx / 2;
        else if(addr > mapping.last_addr)
            middle_idx = middle_idx + middle_idx/2;
        else
        {
            *out_filename = (char*)mapping.filename;
            break;
        }
    }

    // find linenumber
    *out_linenum = info->line_nums[addr];
}
