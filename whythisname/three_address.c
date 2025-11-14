#include <stdio.h>
#include <string.h>
#include <ctype.h>

char stack[100];
int top = -1;

void push(char c) { stack[++top] = c; }
char pop() { return stack[top--]; }
int precedence(char c) {
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

// Convert infix expression to postfix
void infixToPostfix(char infix[], char postfix[]) {
    int i, k = 0;
    char ch;
    for (i = 0; i < strlen(infix); i++) {
        ch = infix[i];
        if (isalnum(ch)) {
            postfix[k++] = ch;
        } else if (ch == '(') {
            push(ch);
        } else if (ch == ')') {
            while (top != -1 && stack[top] != '(')
                postfix[k++] = pop();
            pop(); // remove '('
        } else {
            while (top != -1 && precedence(stack[top]) >= precedence(ch))
                postfix[k++] = pop();
            push(ch);
        }
    }
    while (top != -1)
        postfix[k++] = pop();
    postfix[k] = '\0';
}

// Generate Three Address Code from postfix expression
void generateTAC(char postfix[]) {
    char stack2[100][10];
    int top2 = -1, t = 1;
    for (int i = 0; i < strlen(postfix); i++) {
        char ch = postfix[i];
        if (isalnum(ch)) {
            char s[2] = {ch, '\0'};
            strcpy(stack2[++top2], s);
        } else {
            char op2[10], op1[10], temp[10];
            strcpy(op2, stack2[top2--]);
            strcpy(op1, stack2[top2--]);
            sprintf(temp, "t%d", t++);
            printf("%s = %s %c %s\n", temp, op1, ch, op2);
            strcpy(stack2[++top2], temp);
        }
    }
    printf("Result = %s\n", stack2[top2]);
}

int main() {
    char infix[100], postfix[100];
    printf("Enter an arithmetic expression: ");
    scanf("%s", infix);
    infixToPostfix(infix, postfix);
    printf("\nPostfix: %s\n", postfix);
    printf("\nThree Address Code:\n");
    generateTAC(postfix);
    return 0;
}
