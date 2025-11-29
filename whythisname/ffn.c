#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXP 100
#define ASCII 128
int n;
char lhs[MAXP];
char rhs[MAXP][100];
int present[26];
int firsts[26][ASCII], follows[26][ASCII];
int isNT(char c){ return c>='A' && c<='Z'; }
int main(){
    int i,j,k,changed;
    scanf("%d\n", &n);
    for(i=0;i<n;i++){
        char L, R[100];
        fgets(R, 100, stdin);
        if(R[strlen(R)-1]=='\n') R[strlen(R)-1]=0;
        L = R[0];
        lhs[i] = L;
        char *p = R+1;
        while(*p==' ') p++;
        strcpy(rhs[i], p);
        present[L-'A'] = 1;
    }
    follows[lhs[0]-'A']['$'] = 1;
    do{
        changed = 0;
        for(i=0;i<n;i++){
            int A = lhs[i]-'A';
            char *s = rhs[i];
            if(s[0]==0){ // empty RHS -> epsilon
                if(!firsts[A]['e']) { firsts[A]['e']=1; changed=1; }
                continue;
            }
            int L = strlen(s);
            for(j=0;j<L;j++){
                char sym = s[j];
                if(!isNT(sym)){ // terminal
                    if(!firsts[A][(int)sym]){ firsts[A][(int)sym]=1; changed=1; }
                    break;
                } else { // non-terminal
                    int B = sym - 'A';
                    for(k=0;k<ASCII;k++){
                        if(k=='e') continue;
                        if(firsts[B][k] && !firsts[A][k]){ firsts[A][k]=1; changed=1; }
                    }
                    if(!firsts[B]['e']) break; // can't derive epsilon -> stop
                    if(j==L-1){ // all nullable -> add epsilon
                        if(!firsts[A]['e']){ firsts[A]['e']=1; changed=1; }
                    }
                }
            }
        }
    }while(changed);
    do{
        changed = 0;
        for(i=0;i<n;i++){
            int A = lhs[i]-'A';
            char *s = rhs[i];
            int L = strlen(s);
            for(j=0;j<L;j++){
                if(!isNT(s[j])) continue;
                int B = s[j]-'A';
                int nullable = 1;
                for(k=j+1;k<L;k++){
                    char sym = s[k];
                    if(!isNT(sym)){
                        if(!follows[B][(int)sym]){ follows[B][(int)sym]=1; changed=1; }
                        nullable = 0; break;
                    } else {
                        int C = sym - 'A';
                        for(int c=0;c<ASCII;c++){
                            if(c=='e') continue;
                            if(firsts[C][c] && !follows[B][c]){ follows[B][c]=1; changed=1; }
                        }
                        if(!firsts[C]['e']){ nullable=0; break; }
                    }
                }
                if(nullable){
                    for(k=0;k<ASCII;k++){
                        if(follows[A][k] && !follows[B][k]){ follows[B][k]=1; changed=1; }
                    }
                }
            }
        }
    }while(changed);
    for(i=0;i<26;i++) if(present[i]){
        printf("FIRST(%c) = {", 'A'+i);
        int first_printed=0;
        for(j=0;j<ASCII;j++){
            if(firsts[i][j]){
                if(first_printed) printf(", ");
                if(j=='e') printf("e"); else printf("%c", j);
                first_printed=1;
            }
        }
        printf("}\n");
        printf("FOLLOW(%c) = {", 'A'+i);
        int fol_printed=0;
        for(j=0;j<ASCII;j++){
            if(follows[i][j]){
                if(fol_printed) printf(", ");
                if(j=='$') printf("$"); else printf("%c", j);
                fol_printed=1;
            }
        }
        printf("}\n");
    }
    return 0;
}
