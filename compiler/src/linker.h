#ifndef LINKER_H
#define LINKER_H

#include <vector>
#include "debug_info.h"
#include "gen_bytecode.h"

int link(std::vector<CompiledObj>& compiled_objects, ByteCode* code);
int link_debug(std::vector<CompiledObj>& compiled_objects, ByteCode* code, DebugInfo* dbginfo);

#endif
