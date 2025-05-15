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



TokenList tokenize(const char *source, int source_size);
TokenList init_token_list(int initial_capacity);
void add_token(TokenList *list, Token token);
int is_alpha(char c);
TokenType get_type(char* string);


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

    printf("%s", buffer);

    TokenList tokens = tokenize(buffer, read_size);

    free(buffer);
    for (int i = 0; i < tokens.count; i++) {
        free(tokens.tokens[i].lexeme);
    }
    free(tokens.tokens);  
    fclose(file);
    return 0;
}

TokenList tokenize(const char *source, int source_size){
    char current;
    TokenList list = init_token_list(1);
    for (int i=0; i < source_size; i++){
        current = source[i];
        if (is_alpha(current)){
            int capacity = 16;
            char *buffer = malloc(capacity);  // start with 16 chars
            if (!buffer){
                printf("Out of memory!");
                exit(1);
            }
            int idx = 0;
            
            while (i < source_size && is_alpha(current)) {
                if (idx >= capacity - 1) {  // -1 for null terminator
                    capacity *= 2;
                    buffer = realloc(buffer, capacity);
                    if (!buffer){
                        free(buffer);
                        printf("Out of memory!");
                        exit(1);
                    }
                }
                buffer[idx++] = current;
                current = source[++i];  // advance i and get next char
            }
            current = source[--i];
            buffer[idx] = '\0'; // null-terminate the string

            Token token= {
                            .type = get_type(buffer),
                            .lexeme = buffer
                         };
            add_token(&list, token);
            }
        }
    return list;
}

int is_alpha(char c){
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'){
        return 1;
    }
    return 0;
}

int is_digit(char c){
    if (c >= 0 && c <= 9){
        return 1;
    }
    return 0;
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

TokenType get_type(char *string){
    if (strcmp(string, "return") == 0) {printf("found return!/n"); return _return;}
    return unknown;
}

