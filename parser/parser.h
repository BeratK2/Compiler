#ifndef PARSER_H
#define PARSER_H

typedef enum {
    NUMBER,
    OPERATOR,
    END_OF_INPUT
} TokenType;

typedef struct {
    TokenType type;
    union {
        int number;
        char operator;
    } value;
} Token;

Token getNextToken();
int parseExpression();

#endif
