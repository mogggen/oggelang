#ifndef PARSER_H
#define PARSER_H

#include "block_alloc.h"
#include "lexer.h"
#include "ast.h"

AstStatement* parse(LexerContext& lexer, BlockAlloc& alloc);

#endif
