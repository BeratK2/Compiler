#include <stdio.h>
#include <string.h>
#include "lexer.h"


int main() {
    char input[1000];
    printf("Enter a program code:\n");
    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " \t\n");
    while (token != NULL) {
        struct Token currentToken;
        strncpy(currentToken.value, token, sizeof(currentToken.value));
        currentToken.type = classifyToken(token);

        printf("Token: %s, Type: %d\n", currentToken.value, currentToken.type);

        token = strtok(NULL, " \t\n");
    }

    return 0;
}
