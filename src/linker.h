#ifndef LINKER_H
#define LINKER_H

#include <vector>
#include "compiler.h"

struct ByteCode
{
    int size;
    int code_size;
    int* data;
};

ByteCode link(std::vector<CompiledObj> compiled_objects);

#endif
