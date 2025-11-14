#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 50   // max productions

char LHS[MAX];        // LHS non-terminals
char RHS[MAX][50];    // RHS strings
int n;                // number of productions

bool FIRST[26][256];   // FIRST[A][symbol]
bool FOLLOW[26][256];  // FOLLOW[A][symbol]

bool isNT(char c) {
    return (c >= 'A' && c <= 'Z');
}

int idx(char c) { 
    return c - 'A'; 
}

void addFIRST(int A, char c, bool *changed) {
    if (!FIRST[A][(unsigned char)c]) {
        FIRST[A][(unsigned char)c] = true;
        *changed = true;
    }
}

void addFOLLOW(int A, char c, bool *changed) {
    if (!FOLLOW[A][(unsigned char)c]) {
        FOLLOW[A][(unsigned char)c] = true;
        *changed = true;
    }
}

void computeFIRST() {
    bool changed = true;
    while (changed) {
        changed = false;

        for (int p = 0; p < n; p++) {
            int A = idx(LHS[p]);
            char *alpha = RHS[p];

            // Case: epsilon only RHS "#"
            if (strcmp(alpha, "#") == 0) {
                addFIRST(A, '#', &changed);
                continue;
            }

            bool allNullable = true;

            for (int i = 0; alpha[i] != '\0'; i++) {
                char X = alpha[i];

                if (!isNT(X)) {
                    // Terminal
                    addFIRST(A, X, &changed);
                    allNullable = false;
                    break;
                }

                // X is non-terminal
                for (int c = 0; c < 256; c++) {
                    if (FIRST[idx(X)][c] && c != '#')
                        addFIRST(A, c, &changed);
                }

                if (FIRST[idx(X)]['#']) {
                    // continue to next symbol
                } else {
                    allNullable = false;
                    break;
                }
            }

            if (allNullable)
                addFIRST(A, '#', &changed);
        }
    }
}

void computeFOLLOW(char start) {
    addFOLLOW(idx(start), '$', &(bool){0}); // Start symbol has $

    bool changed = true;
    while (changed) {
        changed = false;

        for (int p = 0; p < n; p++) {
            int A = idx(LHS[p]);
            char *alpha = RHS[p];
            int len = strlen(alpha);

            for (int i = 0; i < len; i++) {
                char B = alpha[i];
                if (!isNT(B)) continue;

                bool allNullable = true;

                for (int j = i + 1; j < len; j++) {
                    char X = alpha[j];

                    if (!isNT(X)) {
                        if (X != '#')
                            addFOLLOW(idx(B), X, &changed);
                        allNullable = false;
                        break;
                    }

                    // X is NT
                    for (int c = 0; c < 256; c++)
                        if (FIRST[idx(X)][c] && c != '#')
                            addFOLLOW(idx(B), c, &changed);

                    if (FIRST[idx(X)]['#']) {
                        allNullable = true;
                        continue;
                    } else {
                        allNullable = false;
                        break;
                    }
                }

                if (i == len - 1 || allNullable) {
                    for (int c = 0; c < 256; c++)
                        if (FOLLOW[A][c])
                            addFOLLOW(idx(B), c, &changed);
                }
            }
        }
    }
}

void printSets() {
    printf("\nFIRST sets:\n");
    for (int i = 0; i < 26; i++) {  
        bool used = false;
        for (int p = 0; p < n; p++)
            if (idx(LHS[p]) == i) used = true;

        if (!used) continue;

        printf("FIRST(%c) = { ", 'A' + i);
        for (int c = 0; c < 256; c++)
            if (FIRST[i][c]) printf("%c ", c);
        printf("}\n");
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < 26; i++) {
        bool used = false;
        for (int p = 0; p < n; p++)
            if (idx(LHS[p]) == i) used = true;

        if (!used) continue;

        printf("FOLLOW(%c) = { ", 'A' + i);
        for (int c = 0; c < 256; c++)
            if (FOLLOW[i][c]) printf("%c ", c);
        printf("}\n");
    }
}

int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);
    getchar();

    printf("Enter productions (example: A->aB):\n");
    for (int i = 0; i < n; i++) {
        char line[100];
        fgets(line, 100, stdin);

        char *arrow = strstr(line, "->");
        LHS[i] = line[0];
        strcpy(RHS[i], arrow + 2);
        RHS[i][strcspn(RHS[i], "\n")] = '\0';
    }

    char start = LHS[0];

    computeFIRST();
    computeFOLLOW(start);
    printSets();

    return 0;
}