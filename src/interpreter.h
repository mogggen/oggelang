#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "opcodes.h"

void print_opcodes(ByteCode code);

void run(ByteCode code);

#endif
