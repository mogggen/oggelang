#ifndef LINKER_H
#define LINKER_H

#include <vector>
#include "compiler.h"

ByteCode link(std::vector<CompiledObj> compiled_objects);

#endif
