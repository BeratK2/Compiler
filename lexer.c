#include <ctype.h>
#include <string.h>
#include "lexer.h"

enum TokenType classifyToken(char token[]) {
    if (isalpha(token[0]) || token[0] == '_') {
        return IDENTIFIER;
    }

    if (isdigit(token[0])) {
        return NUMBER;
    }

    if (strchr("+-*/=^", token[0]) != NULL) {
        return OPERATOR;
    }

    if (strchr("(),;{}[]", token[0]) != NULL) {
        return PUNCTUATION;
    }

    if (strcmp(token, "if") == 0 || strcmp(token, "else") == 0 || strcmp(token, "while") == 0) {
        return KEYWORD;
    }

    return ERROR;
}
