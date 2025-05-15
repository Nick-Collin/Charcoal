#pragma once

#include "token.h"

typedef struct {
    Token *tokens;
    int count;
    int capacity;
} TokenList;

TokenList init_token_list(int initial_capacity);
void add_token(TokenList *list, Token token);
