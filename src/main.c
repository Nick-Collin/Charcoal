#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "tokenlist.h"

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Usage: compiler <File> <Options>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error opening file!");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    char *buffer = malloc(fsize + 1);
    size_t read_size = fread(buffer, 1, fsize, file);
    buffer[read_size] = '\0';

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
