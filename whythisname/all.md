CN
- [BitStuffing](#bitstuffing)
- [ByteStuffing](#bytestuffing)
- [Hamming](#hamming)
- [MinHam](#minham)
- [CheckSum](#checksum)
- [CRC](#crc)
- [DVR](#dvr)
- [LSR](#lsr)
CD
- [LexicalAnalyzer](#lexicalanalyzer)
- [LexProgram](#lexprogram)
- [FirstFollow](#firstfollow)
- [YaccValid](#yaccvalid)
- [YaccOp](#yaccop)
- [ThreeAddress](#threeaddress)
- [MachineCode](#machinecode)


#BitStuffing
```
#include <stdio.h>
#include <string.h>
#define MAX_LEN 1000

void bitStuff(const char *input, char *output) {
    int c = 0;
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        output[j++] = input[i];
        if (input[i] == '1') {
            c++;
            if (c == 5) {
                output[j++] = '0'; // insert a '0' after five consecutive 1s
                c = 0;
            }
        } else {
            c = 0;
        }
    }
    output[j] = '\0';
}

void bitUnstuff(const char *input, char *output) {
    int c = 0;
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        output[j++] = input[i];
        if (input[i] == '1') {
            c++;
            if (c == 5) {
                i++; // skip the stuffed '0'
                c = 0;
            }
        } else {
            c = 0;
        }
    }
    output[j] = '\0';
}

int main() {
    char input[MAX_LEN];
    char stuffed[2 * MAX_LEN];
    char unstuffed[MAX_LEN];

    printf("Enter bit stream: ");
    scanf("%s", input);

    bitStuff(input, stuffed);
    printf("Bit Stuffed: %s\n", stuffed);

    bitUnstuff(stuffed, unstuffed);
    printf("Bit Unstuffed: %s\n", unstuffed);

    return 0;
}
```


#ByteStuffing
```
#include <stdio.h>
#include <string.h>

char stuffed[1000];
char destuffed[1000];
const char FLAG[] = "FLAG";
const char ESC[]  = "ESC";

void byteStuffing(const char input[]) {
    stuffed[0] = '\0';  // initialize
    strcat(stuffed, FLAG); // starting frame delimiter

    int i = 0;
    int in_len = strlen(input);
    while (i < in_len) {
        // If payload contains FLAG
        if (i + strlen(FLAG) <= in_len &&
            strncmp(&input[i], FLAG, strlen(FLAG)) == 0) {
            strcat(stuffed, ESC);
            strcat(stuffed, FLAG);
            i += strlen(FLAG);
        }
        // If payload contains ESC
        else if (i + strlen(ESC) <= in_len &&
                 strncmp(&input[i], ESC, strlen(ESC)) == 0) {
            strcat(stuffed, ESC);
            strcat(stuffed, ESC);
            i += strlen(ESC);
        }
        // Otherwise copy single char
        else {
            int len = strlen(stuffed);
            stuffed[len] = input[i];
            stuffed[len + 1] = '\0';
            i++;
        }
    }

    strcat(stuffed, FLAG); // ending frame delimiter
    printf("After Stuffing: %s\n", stuffed);
}

void byteDestuffing() {
    destuffed[0] = '\0'; // initialize

    int start = strlen(FLAG);
    int end = strlen(stuffed) - strlen(FLAG); // index just after last payload char
    int i = start;
    while (i < end) {
        // If we see ESC in stuffed payload
        if (i + strlen(ESC) <= end && strncmp(&stuffed[i], ESC, strlen(ESC)) == 0) {
            i += strlen(ESC); // skip ESC marker

            // If next is FLAG => this was ESC+FLAG => output FLAG
            if (i + strlen(FLAG) <= end && strncmp(&stuffed[i], FLAG, strlen(FLAG)) == 0) {
                strcat(destuffed, FLAG);
                i += strlen(FLAG);
            }
            // If next is ESC => this was ESC+ESC => output ESC
            else if (i + strlen(ESC) <= end && strncmp(&stuffed[i], ESC, strlen(ESC)) == 0) {
                strcat(destuffed, ESC);
                i += strlen(ESC);
            }
            else {
                // Unexpected sequence after ESC: treat as error or copy literal (here we copy literal)
                int len = strlen(destuffed);
                destuffed[len] = stuffed[i];
                destuffed[len + 1] = '\0';
                i++;
            }
        } else {
            // Normal char copy
            int len = strlen(destuffed);
            destuffed[len] = stuffed[i];
            destuffed[len + 1] = '\0';
            i++;
        }
    }
    printf("After Destuffing: %s\n", destuffed);
}

int main() {
    char input[200];
    printf("Enter input: ");
    if (scanf("%199s", input) != 1) {
        printf("Input error\n");
        return 1;
    }

    byteStuffing(input);
    byteDestuffing();

    return 0;
}
```

#Hamming
```
#include <stdio.h>
#include <string.h>
#define Max_len 9
int hamming_dist(const char *b1,const char *b2){
    int distance=0;
    int l1=strlen(b1);
    int l2=strlen(b2);
    int i;
    if(l1!=l2){
        printf("Error:Binary strings must be of same length");
        return -1;
    }
    for(i=0;i<l1;i++){
        if(b1[i]!=b2[i]){
            distance++;
        }
    }
    return distance;
}
int isBinary(const char *bin){
    int i;
    for(i=0;bin[i]!='\0';i++){
        if(bin[i]!='0'&&bin[i]!='1'){
            return 0;
        }
    }
    return 1;
}
int main(){
    char b1[Max_len];
    char b2[Max_len];
    printf("Enter string1:");
    scanf("%8s",b1);
    printf("Enter string2:");
    scanf("%8s",b2);
    int l1=strlen(b1);
    int l2=strlen(b2);
    if(!((l1==4||l2==8) && l1==l2)){
        printf("Error:strings must be both 4 or both 8 bits long\n");
        return 1;
    }
    if(!isBinary(b1)||!isBinary(b2)){
        printf("Error:strings must be binary");
        return 1;
    }
    int dist=hamming_dist(b1,b2);
    if(dist!=-1){
        printf("Hamming distance = %d\n",dist);
    }
    return 0;
    }
```

#MinHam
```
#include <stdio.h>
#include <string.h>
#define CODEWORDS 8
#define Max_len 9
int hamming_dist(const char *b1,const char *b2){
    int distance=0;
    int l1=strlen(b1);
    int l2=strlen(b2);
    int i;
    if(l1!=l2){
        printf("Error:Binary strings must be of same length");
        return -1;
    }
    for(i=0;i<l1;i++){
        if(b1[i]!=b2[i]){
            distance++;
        }
    }
    return distance;
}
int isBinary(const char *bin){
    int i;
    for(i=0;bin[i]!='\0';i++){
        if(bin[i]!='0'&&bin[i]!='1'){
            return 0;
        }
    }
    return 1;
}
int main(){
    char codewords[CODEWORDS][Max_len];
    int length=0;
    int i,j;
    printf("Enter %d code words (each 4 or 8 bits):\n",CODEWORDS);
    for(i=0;i<CODEWORDS;i++){
        printf("Code word %d:",i+1);
        scanf("%8s",codewords[i]);
        int l1=strlen(codewords[i]);
        if(i==0){
            if(l1!=4&&l1!=8){
                printf("Error:code word length must be 4 or 8 bits\n");
                return 1;
            }
            length=l1;
            }
            else{
                if(l1!=length){
                    printf("Error:All code words must be the same length\n");
                    return 1;
                }
            }
            if(!isBinary(codewords[i])){
                printf("Error:code word must contain only 0s and 1s");
                return 1;
            }
    }
    int min_dist=length+1;
    for(i=0;i<CODEWORDS-1;i++){
        for(j=i+1;j<CODEWORDS;j++){
            int dist=hamming_dist(codewords[i],codewords[j]);
            if(dist<min_dist){
                min_dist=dist;
            }
        }
    }
    printf("Minimum hamming distance among code words:%d\n",min_dist);
    return 0;
}
```

#CheckSum
```
#include <stdio.h>
// binary addition using carry (works for integers)
int binaryAdd(int a, int b) {
    int carry;
    while (b != 0) {
        carry = (a & b) << 1;
        a = a ^ b;
        b = carry;
    }
    return a;
}
// Fold sum into 4 bits with end-around carry
int wrapTo4Bit(int sum) {
    while (sum > 0xF) {
        int overflow = sum >> 4;         // bits beyond 4
        sum = (sum & 0xF) + overflow;   // add overflow back (end-around)
    }
    return sum & 0xF;
}
// Compute checksum: sum all words, wrap to 4-bit, then 1's complement
int computeChecksum(int data[], int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum = binaryAdd(sum, data[i]);
    }
    sum = wrapTo4Bit(sum);
    return (~sum) & 0xF;  // 1's complement in 4 bits
}
int main() {
    int n;
    printf("Enter no of data elements : ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    int sender[n];
    printf("Enter sender data elements (0-255) : ");
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &sender[i]) != 1) sender[i] = 0;
        // reduce to 4-bit word (since we are folding to 4 bits)
        sender[i] &= 0xF;
    }
    int checksum = computeChecksum(sender, n);
    printf("Checksum for the sender : %d\n\n", checksum);
    // Receiver side: read receiver data elements (user may input same or different)
    int receiver[n];
    printf("Enter receiver data elements (0-255) : ");
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &receiver[i]) != 1) receiver[i] = 0;
        receiver[i] &= 0xF;
    }
    // Append sender checksum to receiver data and compute final checksum
    int full[n + 1];
    for (int i = 0; i < n; ++i) full[i] = receiver[i];
    full[n] = checksum;
    int recvChecksum = computeChecksum(full, n + 1);
    printf("Checksum of the receiver : %d\n", recvChecksum);
    if (recvChecksum == 0)
        printf("Correct data transmitted.\n");
    else
        printf("Error in data transmission.\n");
    return 0;
}
```

#CRC
```
#include <stdio.h>
#include <string.h>
char exor(char a, char b) {
    return (a == b) ? '0' : '1';
}
void crc(char data[], char key[]) {
    int datalen = strlen(data);
    int keylen = strlen(key);
    for (int i = 0; i < keylen - 1; i++)
        data[datalen + i] = '0';
    data[datalen + keylen - 1] = '\0';
    int codelen = datalen + keylen - 1;
    char temp[20], rem[20];
    for (int i = 0; i < keylen; i++)
        rem[i] = data[i];
    for (int j = keylen; j <= codelen; j++) {
        for (int i = 0; i < keylen; i++)
            temp[i] = rem[i];
        if (rem[0] == '0') {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = temp[i + 1];
        } else {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = exor(temp[i + 1], key[i + 1]);
        }
        if (j != codelen)
            rem[keylen - 1] = data[j];
        else
            rem[keylen - 1] = '\0';
    }
    for (int i = 0; i < keylen - 1; i++)
        data[datalen + i] = rem[i];
    data[codelen] = '\0';
    printf("CRC = %s\n", rem);
    printf("Dataword with CRC (codeword) = %s\n", data);
}
int checkCRC(char received[], char key[]) {
    int len = strlen(received);
    int keylen = strlen(key);
    char rem[20], temp[20];
    for (int i = 0; i < keylen; i++)
        rem[i] = received[i];
    for (int j = keylen; j <= len; j++) {
        for (int i = 0; i < keylen; i++)
            temp[i] = rem[i];
        if (rem[0] == '0') {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = temp[i + 1];
        } else {
            for (int i = 0; i < keylen - 1; i++)
                rem[i] = exor(temp[i + 1], key[i + 1]);
        }
        if (j != len)
            rem[keylen - 1] = received[j];
        else
            rem[keylen - 1] = '\0';
    }
    for (int i = 0; i < keylen - 1; i++)
        if (rem[i] != '0')
            return 0;
    return 1;
}
int main() {
    char key[20], data[40], received[40];
    printf("Enter the data: ");
    scanf("%s", data);
    printf("Enter the key: ");
    scanf("%s", key);
    crc(data, key);
    printf("\nEnter received data (data + CRC): ");
    scanf("%s", received);
    if (checkCRC(received, key)) {
        printf("No error detected in received data.\n");
        printf("Data is: %c%c%c%c\n", received[0], received[1], received[2], received[3]);
    } else {
        printf("Error detected in received data.\n");
    }
    return 0;
}
```

#DVR
```
#include <stdio.h>
#define INF 9999
#define MAX 26   // supports up to 26 nodes (A-Z)
struct node {
    int dist[MAX];
    int nextHop[MAX];
} rt[MAX];
int main() {
    int costMat[MAX][MAX];
    int nodes;
    printf("Enter number of nodes: ");
    scanf("%d", &nodes);
    printf("\nEnter the cost matrix (use -1 for no direct link):\n");
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            scanf("%d", &costMat[i][j]);
            if (costMat[i][j] == -1 && i != j)
                costMat[i][j] = INF; // convert -1 to INF
            if (i == j)
                costMat[i][j] = 0; // distance to itself = 0
        }
    }
    // Initialize routing tables
    for (int i = 0; i < nodes; i++) {
        for (int j = 0; j < nodes; j++) {
            rt[i].dist[j] = costMat[i][j];
            rt[i].nextHop[j] = j;
        }
    }
    int updated;
    do {
        updated = 0;
        for (int i = 0; i < nodes; i++) {
            for (int j = 0; j < nodes; j++) {
                for (int k = 0; k < nodes; k++) {
                    if (rt[i].dist[j] > costMat[i][k] + rt[k].dist[j]) {
                        rt[i].dist[j] = costMat[i][k] + rt[k].dist[j];
                        rt[i].nextHop[j] = k;
                        updated = 1;
                    }
                }
            }
        }
    } while (updated);
    // Print final routing tables
    for (int i = 0; i < nodes; i++) {
        printf("\nRouting table for node %c:\n", 'A' + i);
        printf("Destination\tDistance\tNext Hop\n");
        for (int j = 0; j < nodes; j++) {
            if (rt[i].dist[j] >= INF)
                printf("%c\t\tINF\t\t-\n", 'A' + j);
            else
                printf("%c\t\t\t%d\t\t\t%c\n", 'A' + j, rt[i].dist[j],
'A' + rt[i].nextHop[j]);
        }
    }
    return 0;
}
```

#LSR
```
#include <stdio.h>
#include <limits.h>

#define INF INT_MAX

int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void dijkstra(int graph[][20], int src, int n) {
    int dist[20];      // shortest distance from src to each node
    int visited[20];   // visited nodes
    int prev[20];      // previous node in shortest path

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < n-1; count++) {
        int u = minDistance(dist, visited, n);
        if(u == -1) break;  // no reachable remaining nodes
        visited[u] = 1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] != 0 && dist[u] != INF
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
    }

    // Print shortest paths
    printf("\nNode\tDistance from Source\tPath\n");
    for (int i = 0; i < n; i++) {
        if(dist[i] == INF) {
            printf("%d\t\tINF\t\tNo path\n", i);
            continue;
        }
        printf("%d\t\t%d\t\t", i, dist[i]);
        int path[20], count = 0;
        int crawl = i;
        path[count++] = crawl;
        while(prev[crawl] != -1) {
            crawl = prev[crawl];
            path[count++] = crawl;
        }
        for(int j = count-1; j >= 0; j--)
            printf("%d ", path[j]);
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter number of nodes: ");
    scanf("%d", &n);

    int graph[20][20];
    printf("Enter adjacency matrix (0 if no direct link):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    int src;
    printf("Enter the source node (0 to %d): ", n-1);
    scanf("%d", &src);

    dijkstra(graph, src, n);

    return 0;
}
```


#LexicalAnalyzer
```
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
```

#LexProgram
```
%{
#include <stdio.h>
%}
%%
"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|
"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|
"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|
"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|
"volatile"|"while"      { printf("Keyword : %s\n", yytext); }
[0-9]+                  { printf("Number : %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("Identifier : %s\n", yytext); }
[ \t\n]+                { /* Ignore white spaces */ }
";"|","|"("|")"|"{"|"}"|"["|"]"   
                        { printf("Punctuation : %s\n", yytext); }
"#"                     { printf("Preprocessor Directive : %s\n", yytext); }
"+"|"-"|"*"|"/"|"="|"<"|">"|"<<"|">>"  
                        { printf("Operator : %s\n", yytext); }
.                       { printf("Unknown : %s\n", yytext); }
%%
int yywrap() { return 1; }
int main() {
    yylex();
    return 0;
}
```

#FirstFollow
```
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
```

#YaccValid
```
expr.y

%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token ID NUM
%left '+' '-'
%left '*' '/'

%%
E : E '+' T
  | E '-' T
  | T
  ;

T : T '*' F
  | T '/' F
  | F
  ;

F : '(' E ')'
  | ID
  | NUM
  ;

%%

void yyerror(const char *s) {
    printf("Invalid expression!\n");
}

int main() {
    printf("Enter an arithmetic expression: ");
    if (yyparse() == 0)
        printf("Valid expression!\n");
    return 0;
}

expr.l

%{
#include "expr.tab.h"
%}

%%
[0-9]+      { return NUM; }
[a-zA-Z]+   { return ID; }
"("         { return '('; }
")"         { return ')'; }
[\+\-\*/]   { return yytext[0]; }
\n          { return 0; }      /* Stop on newline */
[ \t]       ;                  /* Ignore spaces */
.           { return yytext[0]; }
%%

int yywrap() { return 1; }
```

#YaccOp
```
calc.y

%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int yylex(void);
void yyerror(const char *s);
bool error_flag = false;  // track if an error occurred
%}

%token NUM EXIT
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
input:
      /* empty */
    | input line
    ;

line:
      expr '\n'  {
                    if (!error_flag)
                        printf("Result = %d\n", $1);
                    error_flag = false; // reset for next line
                 }
    | EXIT '\n'  { printf("Exiting...\n"); exit(0); }
    | '\n'
    ;

expr:
      expr '+' expr    { $$ = $1 + $3; }
    | expr '-' expr    { $$ = $1 - $3; }
    | expr '*' expr    { $$ = $1 * $3; }
    | expr '/' expr    {
                          if ($3 == 0) {
                              yyerror("Division by zero!");
                              error_flag = true;
                              $$ = 0;  // placeholder (won’t be printed)
                          } else {
                              $$ = $1 / $3;
                          }
                       }
    | '-' expr %prec UMINUS { $$ = -$2; }
    | '(' expr ')'     { $$ = $2; }
    | NUM              { $$ = $1; }
    ;
%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    printf("Enter an arithmetic expression (type 'exit' or '$' to quit):\n");
    yyparse();
    return 0;
}

calc.l

%{
#include "calc.tab.h"
#include <string.h>
%}

%%
[0-9]+          { yylval = atoi(yytext); return NUM; }
[ \t\r]+        ;                /* ignore spaces/tabs */
"exit"          return EXIT;
"$"             return EXIT;
"("             return '(';
")"             return ')';
[\+\-\*/]       { return yytext[0]; }
\n              return '\n';
.               { return yytext[0]; }
%%

int yywrap() { return 1; }
```

#ThreeAddress
```
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
```

#MachineCode
```
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
    scanf("%[^\n]", expr);  // Read full line

    Node* root = buildAST(expr);

    printf("\nGenerated Machine Code:\n");
    char* res = generateCode(root);
    printf("Result stored in %s\n", res);

    free(res);
    return 0;
}
```