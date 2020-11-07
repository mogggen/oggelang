#ifndef LINKER_H
#define LINKER_H

#include <vector>
#include "compiler.h"

int link(std::vector<CompiledObj>& compiled_objects, ByteCode* code);

#endif
