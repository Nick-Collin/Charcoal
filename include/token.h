#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    _return,
    int_literal,
    semicolon,
    unknown
} TokenType;

typedef struct {
    TokenType type;
    char *lexeme;
} Token;

TokenType get_type(char *string);
const char* token_type_to_string(TokenType type);

#endif
