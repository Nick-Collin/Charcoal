#pragma once

#include "tokenlist.h"

typedef struct {
    const char *start;
    const char *current;
    int length;
} Lexer;

TokenList tokenize(const char *source, int source_size);

