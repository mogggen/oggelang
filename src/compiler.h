#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include "opcodes.h"

ByteCode compile(AstStatement* root);

#endif
