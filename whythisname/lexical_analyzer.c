#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
bool isPunc(char ch) {
    return (ch == ';' || ch == ',' || ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' || ch == '#');
}
bool isKeyword(char *str) {
    const char *keywords[] = {
        "auto", "break", "else", "float",
        "return", "struct", "int", "main"
    };
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0)
            return true;
    }
    return false;
}
int main() {
    char lex_inp[1000];
    printf("Enter the program text (end with $):\n");
    int i = 0;
    char ch;
    while ((ch = getchar()) != '$') {
        lex_inp[i++] = ch;
    }
    lex_inp[i] = '\0';
    i = 0;
    char lex[100];
    printf("\nToken\t\tWord\n");
    while (lex_inp[i]) {
        char c = lex_inp[i];
        if (isdigit(c)) {
            int j = 0;
            while (isdigit(lex_inp[i])) {
                lex[j++] = lex_inp[i++];
            }
            lex[j] = '\0';
            printf("Number\t\t%s\n", lex);
        }
        else if (isalpha(c)) {
            int j = 0;
            while (isalnum(lex_inp[i])) {
                lex[j++] = lex_inp[i++];
            }
            lex[j] = '\0';
            if (isKeyword(lex))
                printf("Keyword\t\t%s\n", lex);
            else
                printf("Identifier\t%s\n", lex);
        }
        else if (c == '-' && lex_inp[i+1] == '>') {
            printf("Operator\t->\n");
            i += 2;
        }
        else if (c=='+' || c=='-' || c=='*' || c=='/') {
            printf("Operator\t%c\n", c);
            i++;
        }
        else if (isPunc(c)) {
            printf("Punctuation\t%c\n", c);
            i++;
        }
        else {
            i++;
        }
    }
    return 0;
}
