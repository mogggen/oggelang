#include "linker.h"

#include <stdlib.h>

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

    int* data = (int*)malloc(total_size * sizeof(int));

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
				int offset = 0;
                for(auto other_co : compiled_objects)
                {
					if (other_co.filename_hash == g.filename_hash)
					{
						auto it = other_co.program_line_num.end();
						while (it != other_co.program_line_num.begin()) // can use a binary search here.
						{
							--it;
							if (g.line_num <= it->line_num)
								co.code[g.addr] = it->addr+offset;
						}
					}
					else
						offset += other_co.code.size();
                }
            }
        }

        // fix all variable addresses
        for(auto a : co.var_addr)
            co.code[a] += variable_offset;


        for(int i=0; i<co.code.size(); i++)
            data[code_offset + i] = co.code[i];
        code_offset += co.code.size();
        
        variable_offset += co.nr_variables;

    }

    for(int i=total_code_size; i<total_size; i++)
        data[i] = 0;

    *code = ByteCode{total_size, total_code_size, data};
    return 1;
}
