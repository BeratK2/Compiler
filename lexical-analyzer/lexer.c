#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Token types
typedef enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    PUNCTUATION,
    NUMBER
} TokenType;

// Function to check if a string is a keyword
int is_keyword(char *str) {
    char *keywords[] = {"if", "else", "while", "for", "int", "float", "return"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to classify and print tokens
void print_token(TokenType type, char *token) {
    const char *type_str;
    switch (type) {
        case KEYWORD:
            type_str = "Keyword";
            break;
        case IDENTIFIER:
            type_str = "Identifier";
            break;
        case OPERATOR:
            type_str = "Operator";
            break;
        case PUNCTUATION:
            type_str = "Punctuation";
            break;
        case NUMBER:
            type_str = "Number";
            break;
        default:
            type_str = "Unknown";
            break;
    }
    printf("(%s, %s)\n", type_str, token);
}

// Function to perform lexical analysis
void analyze(char *input) {
    char token[100]; // Maximum token length assumed as 100
    int token_pos = 0;

    int i = 0;
    while (input[i] != '\0') {
        if (isspace(input[i])) {
            i++; // Skip whitespace
            continue;
        }

        // Check if character is part of a token
        if (isalnum(input[i])) {
            token[token_pos++] = input[i++];
        } else if (ispunct(input[i])) {
            // Handle operators and punctuation
            token[token_pos] = '\0'; // Null-terminate the token
            if (token_pos > 0) {
                // Identify and print the token
                if (is_keyword(token)) {
                    print_token(KEYWORD, token);
                } else {
                    print_token(IDENTIFIER, token);
                }
                token_pos = 0; // Reset token buffer
            }

            // Check if the character is a standalone operator or punctuation
            token[0] = input[i++];
            token[1] = '\0';
            print_token(OPERATOR, token);
        } else {
            i++; // Skip any other characters
        }
    }

    // Print any remaining token
    if (token_pos > 0) {
        token[token_pos] = '\0';
        if (isdigit(token[0])) {
            print_token(NUMBER, token);
        } else {
            if (is_keyword(token)) {
                print_token(KEYWORD, token);
            } else {
                print_token(IDENTIFIER, token);
            }
        }
    }
}

int main() {
    char input[1000]; // Maximum input size assumed as 1000 characters

    printf("Enter input (press Ctrl+D to end):\n");

    // Read input until EOF (Ctrl+D on Unix/Linux, Ctrl+Z on Windows)
    while (fgets(input, sizeof(input), stdin) != NULL) {
        analyze(input); // Perform lexical analysis on the input
    }

    return 0;
}
