#ifndef LEXER_H
#define LEXER_H

// Define token types
enum TokenType {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    KEYWORD,
    ERROR
};

// Structure to represent a token
struct Token {
    enum TokenType type;
    char value[50];
};

// Function declaration
enum TokenType classifyToken(char token[]);

#endif  // LEXER_H
