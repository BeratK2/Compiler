#include <stdio.h>
#include "lexer.h"
#include <string.h>

int main() {
    char input[1000];
    printf("Enter a program code:\n");
    fgets(input, sizeof(input), stdin);

    char currentToken[50] = "";
    int tokenIndex = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
            // Whitespace encountered, process the current token
            if (tokenIndex > 0) {
                currentToken[tokenIndex] = '\0'; // Null-terminate the token
                struct Token token;
                strncpy(token.value, currentToken, sizeof(token.value));
                token.type = classifyToken(currentToken);

                // Print the token and its type
                printf("Token: %s, Type: %d\n", token.value, token.type);

                // Reset for the next token
                tokenIndex = 0;
            }
        } else {
            // Non-whitespace character, add it to the current token
            if (tokenIndex < sizeof(currentToken) - 1) {
                currentToken[tokenIndex++] = input[i];
            } else {
                // Handle token length overflow (you might want to extend this logic)
                fprintf(stderr, "Error: Token length exceeds buffer size.\n");
                return 1;
            }
        }
    }

    // Process the last token if any
    if (tokenIndex > 0) {
        currentToken[tokenIndex] = '\0'; // Null-terminate the token
        struct Token token;
        strncpy(token.value, currentToken, sizeof(token.value));
        token.type = classifyToken(currentToken);

        // Print the token and its type
        printf("Token: %s, Type: %d\n", token.value, token.type);
    }

    return 0;
}
