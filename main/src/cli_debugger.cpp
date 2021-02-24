#include "cli_debugger.h"

#include <stdio.h>

void run_cli_debugger(DebugState* state)
{
    char buff[512];

    while(state->is_running)
    {
        OpCode current_opcode = (OpCode)state->code->data[state->pc];
        opcode_to_string(&current_opcode, buff);
        printf("-> %s", buff);
        scanf("");
    }

    printf("Program finished.\n");
}
