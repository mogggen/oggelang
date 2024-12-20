#ifndef COMPILER_H
#define COMPILER_H

#include "opcodes.h"
#include "debug_info.h"

int compile_program(ByteCode* out_code, const char* filename, bool print_ast, DebugInfo* out_dbginfo);

#endif
