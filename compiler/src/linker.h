#ifndef LINKER_H
#define LINKER_H

#include <vector>
#include "gen_bytecode.h"

int link(std::vector<CompiledObj>& compiled_objects, ByteCode* code);

#endif
