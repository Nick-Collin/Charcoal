#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

static int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static int is_digit(char c) {
    return c >= '0' && c <= '9';
}

static int is_alnum(char c) {
    return is_alpha(c) || is_digit(c);
}

static int is_at_end(Lexer *lexer) {
    return *(lexer->current) == '\0';
}

static char advance(Lexer *lexer) {
    return *(lexer->current++);
}

static char peek(Lexer *lexer) {
    return *(lexer->current);
}

static void skip_whitespace(Lexer *lexer) {
    while (peek(lexer) == ' ' || peek(lexer) == '\t' || peek(lexer) == '\n' || peek(lexer) == '\r') {
        advance(lexer);
    }
}

static Token scan_identifier(Lexer *lexer, char first_char) {
    int capacity = 16;
    char *buffer = malloc(capacity);
    int length = 0;
    buffer[length++] = first_char;

    while (is_alnum(peek(lexer))) {
        if (length >= capacity - 1) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
        }
        buffer[length++] = advance(lexer);
    }
    buffer[length] = '\0';

    Token token = {.type = get_type(buffer), .lexeme = buffer};
    return token;
}

static Token scan_number(Lexer *lexer, char first_char) {
    int capacity = 4;
    char *buffer = malloc(capacity);
    int length = 0;
    buffer[length++] = first_char;

    while (is_digit(peek(lexer))) {
        if (length >= capacity - 1) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
        }
        buffer[length++] = advance(lexer);
    }
    buffer[length] = '\0';

    Token token = {.type = int_literal, .lexeme = buffer};
    return token;
}

static Token scan_token(Lexer *lexer) {
    char c = advance(lexer);

    if (is_alpha(c)) return scan_identifier(lexer, c);
    if (is_digit(c)) return scan_number(lexer, c);
    if (c == ';') return (Token){.type = semicolon, .lexeme = strdup(";")};

    char lex[2] = {c, '\0'};
    return (Token){.type = unknown, .lexeme = strdup(lex)};
}

TokenList tokenize(const char *source, int source_size) {
    TokenList list = init_token_list(8);

    Lexer lexer = {
        .start = source,
        .current = source,
        .length = source_size
    };

    while (!is_at_end(&lexer)) {
        skip_whitespace(&lexer);
        if (is_at_end(&lexer)) break;

        lexer.start = lexer.current;
        Token token = scan_token(&lexer);
        add_token(&list, token);
    }

    return list;
}
