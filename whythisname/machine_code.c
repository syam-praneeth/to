#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// AST Node Structure
typedef struct node {
    char op;          // Operator if internal node
    char value;       // Operand if leaf node
    struct node *left, *right;
} Node;

// Node stack
Node* nodeStack[100];
int nodeTop = -1;
void pushNode(Node* n) { nodeStack[++nodeTop] = n; }
Node* popNode() { return nodeStack[nodeTop--]; }

// Operator stack
char opStack[100];
int opTop = -1;
void pushOp(char c) { opStack[++opTop] = c; }
char popOp() { return opStack[opTop--]; }
char peekOp() { return opStack[opTop]; }

int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Create leaf node
Node* makeLeaf(char value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->op = '\0';
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Create internal operator node
Node* makeNode(char op, Node* left, Node* right) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->op = op;
    newNode->value = '\0';
    newNode->left = left;
    newNode->right = right;
    return newNode;
}

// Build AST from infix expression
Node* buildAST(char* expr) {
    int len = strlen(expr);
    for (int i = 0; i < len; i++) {
        char ch = expr[i];
        if (isspace(ch)) continue;

        if (isalnum(ch)) {
            pushNode(makeLeaf(ch));
        } else if (ch == '(') {
            pushOp(ch);
        } else if (ch == ')') {
            while (opTop != -1 && peekOp() != '(') {
                char op = popOp();
                Node* right = popNode();
                Node* left = popNode();
                pushNode(makeNode(op, left, right));
            }
            popOp(); // remove '('
        } else { // Operator
            while (opTop != -1 && precedence(peekOp()) >= precedence(ch)) {
                char op = popOp();
                Node* right = popNode();
                Node* left = popNode();
                pushNode(makeNode(op, left, right));
            }
            pushOp(ch);
        }
    }

    while (opTop != -1) {
        char op = popOp();
        Node* right = popNode();
        Node* left = popNode();
        pushNode(makeNode(op, left, right));
    }

    return popNode();
}

// Generate machine code (returns dynamically allocated result name)
int tempCount = 1;
char* generateCode(Node* root) {
    if (root == NULL) return NULL;

    if (root->op == '\0') { // Operand node
        char* val = (char*)malloc(5);
        sprintf(val, "%c", root->value);
        return val;
    }

    char *left = generateCode(root->left);
    char *right = generateCode(root->right);

    char* result = (char*)malloc(5);
    sprintf(result, "T%d", tempCount++);

    printf("LOAD %s\n", left);
    switch (root->op) {
        case '+': printf("ADD %s\n", right); break;
        case '-': printf("SUB %s\n", right); break;
        case '*': printf("MUL %s\n", right); break;
        case '/': printf("DIV %s\n", right); break;
    }
    printf("STORE %s\n", result);

    free(left);
    free(right);
    return result;
}

int main() {
    char expr[100];
    printf("Enter an arithmetic expression: ");
    scanf("%[^\n]", expr);  // Read full line until newline

    Node* root = buildAST(expr);

    printf("\nGenerated Machine Code:\n");
    char* res = generateCode(root);
    printf("Result stored in %s\n", res);

    free(res);
    return 0;
}
