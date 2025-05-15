#include <string.h>
#include "token.h"

TokenType get_type(char *string){
    if (strcmp(string, "return") == 0) return _return;
    return unknown;
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case _return: return "return";
        case int_literal: return "int_literal";
        case semicolon: return "semicolon";
        case unknown: return "unknown";
        default: return "invalid";
    }
}
