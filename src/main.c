#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
    Token *tokens;
    int count;     // number of tokens in use
    int capacity;  // allocated size
} TokenList;

typedef struct 
{
    const char *start;
    const char *current;
    int length;
}Lexer;

TokenList tokenize(const char *source, int source_size);
TokenList init_token_list(int initial_capacity);
void add_token(TokenList *list, Token token);
int is_alpha(char c);
int is_at_end(Lexer *lexer);
char advance(Lexer *lexer);
char peek(Lexer *lexer);
void skip_whitespace(Lexer *lexer);
Token scan_token(Lexer *lexer);
Token scan_identifier(Lexer *lexer, char first_char);
Token scan_number(Lexer *lexer, char first_char);
TokenType get_type(char *string);
const char* token_type_to_string(TokenType type);


int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Usage: compiler <File> <Options>\n");
        return 1;
    }

    //Open file in reading mode
    FILE* file = fopen(argv[1], "r");
    if (!file){
        printf("Error opening file!");
        exit(1);
    }

    //Get file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    //Allocate buffer (+1 for null terminator pointer)
    char *buffer = (char *) malloc(fsize + 1);

    // Read file into buffer
    size_t read_size = fread(buffer, 1, fsize, file);
    buffer[read_size] = '\0';  // Null-terminate the buffer

    printf("File Content:\n%s\n", buffer);

    TokenList tokens = tokenize(buffer, read_size);

    free(buffer);
    for (int i = 0; i < tokens.count; i++) {
        printf("token: %d, type: %s, lexeme: %s\n", i, token_type_to_string(tokens.tokens[i].type), tokens.tokens[i].lexeme);
        free(tokens.tokens[i].lexeme);
    }
    free(tokens.tokens);  
    fclose(file);
    return 0;
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

int is_at_end(Lexer *lexer){
    return *(lexer->current) == '\0';
}

char advance(Lexer *lexer) {
    return *(lexer->current++);
}

char peek(Lexer *lexer) {
    return *(lexer->current);
}

int is_alpha(char c){
   return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

int is_digit(char c){
    return c >= '0' && c <= '9';
}

int is_alnum(char c){
    return is_alpha(c) || is_digit(c);
}

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
        if (!list->tokens){
            printf("Out of memory!");
            exit(1);
        }
    }
    list->tokens[list->count++] = token;
}

Token scan_token(Lexer *lexer) {
    char c = advance(lexer);

    if (is_alpha(c)) {
        return scan_identifier(lexer, c);
    }
    if (is_digit(c)) {
        return scan_number(lexer, c);
    }
    if (c == ';') {
        Token token = {.type = semicolon, .lexeme = strdup(";")};
        return token;
    }

    // Fallback
    char lex[2];
    lex[0] = c;
    lex[1] = '\0';
    Token token = {.type = unknown, .lexeme = strdup(lex)}; // heap-allocated    
    return token;
}

Token scan_identifier(Lexer *lexer, char first_char) {
    int capacity = 16;
    char *buffer = malloc(capacity);
    if (!buffer){
        printf("Out of memory!");
        exit(1);
    }
    int length = 0;
    buffer[length++] = first_char;

    while (is_alnum(peek(lexer))) {
        if (length >= capacity - 1) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
            if (!buffer){
                printf("Out of memory!");
                exit(1);
            }
        }
        buffer[length++] = advance(lexer);
    }
    buffer[length] = '\0';

    TokenType type = get_type(buffer);
    Token token = {.type = type, .lexeme = buffer};
    return token;
}

Token scan_number(Lexer *lexer, char first_char) {
    int capacity = 4;
    char *buffer = malloc(capacity);
    if (!buffer){
        printf("Out of memory!");
        exit(1);
    }
    int length = 0;
    buffer[length++] = first_char;

    while (is_digit(peek(lexer))) {
        if (length >= capacity - 1) {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
            if (!buffer){
                printf("Out of memory!");
                exit(1);
            }
        }
        buffer[length++] = advance(lexer);
    }
    buffer[length] = '\0';

    Token token = {.type = int_literal, .lexeme = buffer};
    return token;
}

TokenType get_type(char *string){
    if (strcmp(string, "return") == 0) return _return;
    return unknown;
}

void skip_whitespace(Lexer *lexer) {
    while (peek(lexer) == ' ' || peek(lexer) == '\t' || peek(lexer) == '\n' || peek(lexer) == '\r') {
        advance(lexer);
    }
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
