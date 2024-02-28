#include <stdio.h>
#include <stdlib.h>

// Enumeration for token types
typedef enum {
    NUMBER,
    OPERATOR,
    END_OF_INPUT
} TokenType;

// Structure to represent a token
typedef struct {
    TokenType type;
    union {
        int number;     // For NUMBER type
        char operator;  // For OPERATOR type
    } value;
} Token;

// Function to get the next token from the input
Token getNextToken() {
    Token token;
    char input[10];  // Assuming the maximum length of input is 10 characters

    printf("Enter a token (number or operator): ");
    scanf("%s", input);

    // Check if the input is a number
    if (input[0] >= '0' && input[0] <= '9') {
        token.type = NUMBER;
        token.value.number = atoi(input);
    } else if (input[0] == '+' || input[0] == '-' || input[0] == '*' || input[0] == '/') {
        token.type = OPERATOR;
        token.value.operator = input[0];
    } else {
        token.type = END_OF_INPUT;
    }

    return token;
}

// Function to parse a factor
int parseFactor() {
    Token token = getNextToken();
    int result;

    if (token.type == NUMBER) {
        result = token.value.number;
    } else if (token.type == OPERATOR && token.value.operator == '(') {
        result = parseExpression();
        // After parsing the expression within parentheses, expect a closing parenthesis
        if (getNextToken().type != OPERATOR || getNextToken().value.operator != ')') {
            printf("Error: Missing closing parenthesis\n");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Error: Invalid factor\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

// Function to parse a term
int parseTerm() {
    int result = parseFactor();
    Token token;

    while (1) {
        token = getNextToken();
        if (token.type == OPERATOR && (token.value.operator == '*' || token.value.operator == '/')) {
            int nextFactor = parseFactor();
            if (token.value.operator == '*') {
                result *= nextFactor;
            } else {
                result /= nextFactor;
            }
        } else {
            // If the token is not an operator, put it back for the next function to handle
            ungetc(token.value.operator, stdin);
            break;
        }
    }

    return result;
}

// Function to parse an expression
int parseExpression() {
    int result = parseTerm();
    Token token;

    while (1) {
        token = getNextToken();
        if (token.type == OPERATOR && (token.value.operator == '+' || token.value.operator == '-')) {
            int nextTerm = parseTerm();
            if (token.value.operator == '+') {
                result += nextTerm;
            } else {
                result -= nextTerm;
            }
        } else {
            // If the token is not an operator, put it back for the next function to handle
            ungetc(token.value.operator, stdin);
            break;
        }
    }

    return result;
}
