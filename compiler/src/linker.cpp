#include "linker.h"

#include <stdlib.h>

int link_debug(std::vector<CompiledObj>& compiled_objects, ByteCode* code, DebugInfo* dbginfo)
{
    int total_size = 0;
    int total_code_size = 0;
    int total_var_size = 0;
    
    for(auto co : compiled_objects)
    {
        total_code_size += co.code.size();
        total_var_size += co.nr_variables;
    }

    total_size = total_code_size + total_var_size;

    int code_offset = 0;
    int variable_offset = 0;

    int* data = (int*)malloc(total_size * sizeof(int));
    int var_start_idx = total_code_size;
    int co_index = 0;

    // construct dbginfo
    dbginfo->n_variables = total_var_size;
    dbginfo->variables = (DebugVar*)malloc(sizeof(DebugVar)*total_var_size);
    dbginfo->line_nums = (int*)malloc(sizeof(int)*total_code_size);
    dbginfo->n_filenames = compiled_objects.size();
    dbginfo->filename_mapping = (FilenameMapping*)malloc(sizeof(FilenameMapping)*compiled_objects.size());

    for(auto co : compiled_objects)
    {
        // fix all goto addresses
        for(auto g : co.goto_addr)
        {
            if(g.filename_hash == co.filename_hash) // goto somewhere in the same file
            {
                for(int i=0; i<co.program_line_num.size(); i++) // can use binary search here
                {
                    if(g.line_num == co.program_line_num[i])
                    {
                        co.code[g.addr] = i;
                        break;
                    }
                }
            }
            else // goto somewhere in another file
            {
				int offset = 0;
                for(auto other_co : compiled_objects)
                {
					if (other_co.filename_hash == g.filename_hash) // find correct file
					{
                        for(int i=0; i<other_co.program_line_num.size(); i++) // can use binary search here
                        {
                            if(g.line_num == other_co.program_line_num[i])
                            {
                                co.code[g.addr] = offset+i;
                                break;
                            }
                        }
					}
					else
						offset += other_co.code.size();
                }
            }
        }

        // fix all variable addresses
        for(auto a : co.var_addr)
            co.code[a] += var_start_idx + variable_offset;

        // debug var names
        for(int i=0; i<co.nr_variables; i++)
            dbginfo->variables[variable_offset+i] = DebugVar{co.var_names[i].name, var_start_idx+variable_offset+i, co.var_names[i].loc};

        // debug line num
        for(int i=0; i<co.program_line_num.size(); i++)
            dbginfo->line_nums[code_offset+i] = co.program_line_num[i];

        // filename_mapping
        dbginfo->filename_mapping[co_index] = FilenameMapping{code_offset, code_offset+(int)co.code.size()-1, co.filename};

        // copy code to the data blob
        for(int i=0; i<co.code.size(); i++)
            data[code_offset + i] = co.code[i];

        code_offset += co.code.size();
        
        // and reserve space for the variables
        variable_offset += co.nr_variables;

        co_index++;
    }

    // set all variables to 0
    for(int i=total_code_size; i<total_size; i++)
        data[i] = 0;

    *code = ByteCode{total_size, total_code_size, data};

    return 1;
}

int link(std::vector<CompiledObj>& compiled_objects, ByteCode* code)
{
    DebugInfo _info;
    int ret = link_debug(compiled_objects, code, &_info);
    free(_info.variables);
    free(_info.line_nums);
    free(_info.filename_mapping);
    return ret;
}
