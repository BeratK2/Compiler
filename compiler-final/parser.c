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

// Parse tree node structure
typedef struct ParseTreeNode {
    char* value;
    struct ParseTreeNode* left;
    struct ParseTreeNode* right;
} ParseTreeNode;

// Function prototypes
int hashFunction(char* str);
TokenHashTable* createTokenHashTable();
void insertToken(TokenHashTable* hashTable, char* value, char* type);
void printTokenHashTable(TokenHashTable* hashTable);
ParseTreeNode* buildParseTree(TokenHashTable* hashTable);
void printParseTree(ParseTreeNode* root);
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

// Build parse tree from token hash table
ParseTreeNode* buildParseTree(TokenHashTable* hashTable) {
    // Use the first token as the root of the parse tree
    ParseTreeNode* root = NULL;

    for (int i = 0; i < TABLE_SIZE; i++) {
        TokenEntry* current = hashTable->table[i];
        while (current != NULL) {
            if (root == NULL) {
                root = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
                root->value = strdup(current->value);
                root->left = NULL;
                root->right = NULL;
            } else {
                // In a real parser, you would build a proper parse tree structure here
                // For simplicity, just add tokens to the right of the root
                ParseTreeNode* newNode = (ParseTreeNode*)malloc(sizeof(ParseTreeNode));
                newNode->value = strdup(current->value);
                newNode->left = NULL;
                newNode->right = root->right;
                root->right = newNode;
            }
            current = current->next;
        }
    }

    return root;
}

// Print parse tree (in-order traversal)
void printParseTree(ParseTreeNode* root) {
    if (root == NULL) {
        return;
    }

    printParseTree(root->left);
    printf("%s ", root->value);
    printParseTree(root->right);
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

// Free memory used by token hash table
void freeTokenHashTable(TokenHashTable* hashTable) {
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
}

// Free memory used by parse tree
void freeParseTree(ParseTreeNode* root) {
    if (root == NULL) {
        return;
    }

    freeParseTree(root->left);
    freeParseTree(root->right);
    free(root->value);
    free(root);
}

// Driver code
int main() {
    char input[100];

    // Prompt user to enter an expression
    printf("Enter an expression: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character

    // Create token hash table and tokenize the input expression
    TokenHashTable* hashTable = createTokenHashTable();
    char* token = strtok(input, " "); // Tokenize by space

    while (token != NULL) {
        // Insert token into hash table (for simplicity, assume all tokens are identifiers)
        insertToken(hashTable, token, "Identifier");
        token = strtok(NULL, " ");
    }

    // Print token hash table
    printTokenHashTable(hashTable);

    // Build parse tree from token hash table
    ParseTreeNode* parseTree = buildParseTree(hashTable);

    // Print parse tree (in-order traversal)
    printf("\nParse Tree: ");
    printParseTree(parseTree);
    printf("\n");

    // Free memory used by token hash table and parse tree
    freeTokenHashTable(hashTable);
    freeParseTree(parseTree);

    return 0;
}