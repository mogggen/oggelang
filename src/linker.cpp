#include "linker.h"

int link(std::vector<CompiledObj>& compiled_objects, ByteCode* code)
{
    int total_size = 0;
    int total_code_size = 0;
    
    for(auto co : compiled_objects)
    {
        total_code_size += co.code.size();
        total_size += co.code.size() + co.nr_variables;
    }

    int code_offset = 0;
    int variable_offset = total_code_size;

    int* data = new int[total_size];

    for(auto co : compiled_objects)
    {
        // fix all goto addresses
        for(auto g : co.goto_addr)
        {
            if(g.filename_hash == co.filename_hash)
            {
                auto it = co.program_line_num.end();
                while(it != co.program_line_num.begin()) // can use a binary search here.
                {
                    --it;
                    if(g.line_num <= it->line_num)
                        co.code[g.addr] = it->addr;
                }
            }
            else
            {
                for(auto other_co : compiled_objects)
                {
                    if(other_co.filename_hash == g.filename_hash)
                    {
                        auto it = other_co.program_line_num.end();
                        while(it != other_co.program_line_num.begin()) // can use a binary search here.
                        {
                            --it;
                            if(g.line_num <= it->line_num)
                                co.code[g.addr] = it->addr;
                        }
                    }
                }
            }
        }

        // fix all variable addresses
        for(auto a : co.var_addr)
            co.code[a] += co.code.size();


        for(int i=0; i<co.code.size(); i++)
            data[code_offset + i] = co.code[i];
        code_offset += co.code.size();
        
        for(int i=0; i<co.nr_variables; i++)
            data[variable_offset + i + co.code.size()] = 0;
        variable_offset += co.nr_variables;

    }

    *code = ByteCode{total_size, total_code_size, data};
    return 1;
}
