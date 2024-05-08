#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define TABLE_SIZE 101 // Size of the hash table 

// Structure for a token entry in the hash table
typedef struct TokenEntry {
    char* value;
    char* type;
    struct TokenEntry* next; // Pointer to handle collisions
} TokenEntry;

// Hash table structure
typedef struct {
    TokenEntry* table[TABLE_SIZE];
} TokenHashTable;

// Function prototypes
int hashFunction(char* str);
TokenHashTable* createTokenHashTable();
void insertToken(TokenHashTable* hashTable, char* value, char* type);
void printTokenHashTable(TokenHashTable* hashTable);
bool isDelimiter(char chr);
bool isOperator(char chr);
bool isValidIdentifier(char* str);
bool isKeyword(char* str);
bool isInteger(char* str);
char* getSubstring(char* str, int start, int end);
int lexicalAnalyzer(char* input, TokenHashTable* hashTable);

// Hash function to compute index in the hash table
int hashFunction(char* str) {
    int hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash * 31 + str[i]) % TABLE_SIZE;
    }
    return hash;
}

// Function to create a new token hash table
TokenHashTable* createTokenHashTable() {
    TokenHashTable* hashTable = (TokenHashTable*)malloc(sizeof(TokenHashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

// Function to insert a token into the hash table
void insertToken(TokenHashTable* hashTable, char* value, char* type) {
    int index = hashFunction(value);

    TokenEntry* newEntry = (TokenEntry*)malloc(sizeof(TokenEntry));
    newEntry->value = strdup(value);
    newEntry->type = strdup(type);
    newEntry->next = NULL;

    // Insert into hash table at the beginning of the linked list (chaining for collision handling)
    newEntry->next = hashTable->table[index];
    hashTable->table[index] = newEntry;
}

// Function to print the token hash table
void printTokenHashTable(TokenHashTable* hashTable) {
    printf("\nToken Hash Table:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        TokenEntry* current = hashTable->table[i];
        while (current != NULL) {
            printf("[%s] -> {Value: %s, Type: %s}\n", current->value, current->value, current->type);
            current = current->next;
        }
    }
}

bool isDelimiter(char chr) {
    return (chr == ' ' || chr == '+' || chr == '-' || chr == '*' || chr == '/' ||
            chr == ',' || chr == ';' || chr == '%' || chr == '>' || chr == '<' ||
            chr == '=' || chr == '(' || chr == ')' || chr == '[' || chr == ']' ||
            chr == '{' || chr == '}');
}

bool isOperator(char chr) {
    return (chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '>' || chr == '<' || chr == '=');
}

bool isValidIdentifier(char *str) {
    return (str[0] != '0' && str[0] != '1' && str[0] != '2' && str[0] != '3' && str[0] != '4' &&
            str[0] != '5' && str[0] != '6' && str[0] != '7' && str[0] != '8' && str[0] != '9' &&
            !isDelimiter(str[0]));
}

bool isKeyword(char *str) {
    const char *keywords[] = {"auto",   "break",    "case",    "char",   "const",  "continue", "default",
                              "do",     "double",   "else",    "enum",   "extern", "float",    "for",
                              "goto",   "if",       "int",     "long",   "register", "return",   "short",
                              "signed", "sizeof",   "static",  "struct", "switch",  "typedef",  "union",
                              "unsigned", "void",   "volatile", "while"};

    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isInteger(char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }
    int i = 0;
    while (isdigit(str[i])) {
        i++;
    }
    return str[i] == '\0';
}

char *getSubstring(char *str, int start, int end) {
    int subLength = end - start + 1;
    char *subStr = (char *)malloc((subLength + 1) * sizeof(char));
    strncpy(subStr, str + start, subLength);
    subStr[subLength] = '\0';
    return subStr;
}

// Main lexical analyzer function
int lexicalAnalyzer(char* input, TokenHashTable* hashTable) {
    int left = 0, right = 0;
    int len = strlen(input);

    while (right <= len && left <= right) {
        if (!isDelimiter(input[right]))
            right++;

        if (isDelimiter(input[right]) && left == right) {
            if (isOperator(input[right])) {
                printf("Token: Operator, Value: %c\n", input[right]);
                insertToken(hashTable, &input[right], "Operator");
            }
            right++;
            left = right;
        } else if (isDelimiter(input[right]) && left != right || (right == len && left != right)) {
            char* subStr = getSubstring(input, left, right - 1);

            if (isKeyword(subStr)) {
                printf("Token: Keyword, Value: %s\n", subStr);
                insertToken(hashTable, subStr, "Keyword");
            } else if (isInteger(subStr)) {
                printf("Token: Integer, Value: %s\n", subStr);
                insertToken(hashTable, subStr, "Integer");
            } else if (isValidIdentifier(subStr) && !isDelimiter(input[right - 1])) {
                printf("Token: Identifier, Value: %s\n", subStr);
                insertToken(hashTable, subStr, "Identifier");
            } else if (!isValidIdentifier(subStr) && !isDelimiter(input[right - 1])) {
                printf("Token: Unidentified, Value: %s\n", subStr);
                insertToken(hashTable, subStr, "Unidentified");
            }
            left = right;
        }
    }
    return 0;
}

// Driver code
int main() {
    char lex_input[MAX_LENGTH];
    printf("Enter an expression: ");
    fgets(lex_input, sizeof(lex_input), stdin);
    lex_input[strcspn(lex_input, "\n")] = '\0'; // Remove newline character

    TokenHashTable* hashTable = createTokenHashTable();

    printf("For Expression \"%s\":\n", lex_input);
    lexicalAnalyzer(lex_input, hashTable);

    printTokenHashTable(hashTable);

    // Free memory used by hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        TokenEntry* current = hashTable->table[i];
        while (current != NULL) {
            TokenEntry* temp = current;
            current = current->next;
            free(temp->value);
            free(temp->type);
            free(temp);
        }
    }
    free(hashTable);

    return 0;
}
