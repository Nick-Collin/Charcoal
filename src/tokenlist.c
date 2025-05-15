#include <stdio.h>
#include <stdlib.h>
#include "tokenlist.h"

TokenList init_token_list(int initial_capacity) {
    TokenList list;
    list.tokens = malloc(sizeof(Token) * initial_capacity);
    list.count = 0;
    list.capacity = initial_capacity;
    return list;
}

void add_token(TokenList *list, Token token) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->tokens = realloc(list->tokens, sizeof(Token) * list->capacity);
        if (!list->tokens) {
            printf("Out of memory!");
            exit(1);
        }
    }
    list->tokens[list->count++] = token;
}
