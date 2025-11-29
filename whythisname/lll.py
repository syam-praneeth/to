# Global variables
lhs = []
rhs = []
present = [0] * 26
firsts = [[0] * 128 for _ in range(26)]
follows = [[0] * 128 for _ in range(26)]
terminals = set()
non_terminals = set()
table = {}

def is_nt(c):
    return 'A' <= c <= 'Z'

def parse_grammar(productions):
    global lhs, rhs, present, terminals, non_terminals
    
    for prod in productions:
        parts = prod.split(maxsplit=1)
        L = parts[0]
        R = parts[1] if len(parts) > 1 else ""
        
        lhs.append(L)
        rhs.append(R)
        present[ord(L) - ord('A')] = 1
        non_terminals.add(L)
        
        # Extract terminals
        for c in R:
            if not is_nt(c) and c != 'e' and c != ' ':
                terminals.add(c)
    
    terminals.add('$')

def compute_first():
    global firsts
    
    n = len(lhs)
    changed = 1
    
    while changed:
        changed = 0
        for i in range(n):
            A = ord(lhs[i]) - ord('A')
            s = rhs[i]
            
            if len(s) == 0:
                if not firsts[A][ord('e')]:
                    firsts[A][ord('e')] = 1
                    changed = 1
                continue
            
            L = len(s)
            j = 0
            while j < L:
                sym = s[j]
                
                if not is_nt(sym):  # terminal
                    if not firsts[A][ord(sym)]:
                        firsts[A][ord(sym)] = 1
                        changed = 1
                    break
                else:  # non-terminal
                    B = ord(sym) - ord('A')
                    for k in range(128):
                        if k == ord('e'):
                            continue
                        if firsts[B][k] and not firsts[A][k]:
                            firsts[A][k] = 1
                            changed = 1
                    
                    if not firsts[B][ord('e')]:
                        break
                    
                    if j == L - 1:  # all nullable
                        if not firsts[A][ord('e')]:
                            firsts[A][ord('e')] = 1
                            changed = 1
                j += 1

def compute_follow():
    """Compute FOLLOW sets"""
    global follows
    
    # Add $ to FOLLOW of start symbol
    follows[ord(lhs[0]) - ord('A')][ord('$')] = 1
    
    n = len(lhs)
    changed = 1
    
    while changed:
        changed = 0
        for i in range(n):
            A = ord(lhs[i]) - ord('A')
            s = rhs[i]
            L = len(s)
            
            for j in range(L):
                if not is_nt(s[j]):
                    continue
                
                B = ord(s[j]) - ord('A')
                nullable = 1
                
                for k in range(j + 1, L):
                    sym = s[k]
                    
                    if not is_nt(sym):
                        if not follows[B][ord(sym)]:
                            follows[B][ord(sym)] = 1
                            changed = 1
                        nullable = 0
                        break
                    else:
                        C = ord(sym) - ord('A')
                        for c in range(128):
                            if c == ord('e'):
                                continue
                            if firsts[C][c] and not follows[B][c]:
                                follows[B][c] = 1
                                changed = 1
                        
                        if not firsts[C][ord('e')]:
                            nullable = 0
                            break
                
                if nullable:
                    for k in range(128):
                        if follows[A][k] and not follows[B][k]:
                            follows[B][k] = 1
                            changed = 1

def construct_table():
    """Construct LL(1) parsing table"""
    global table
    
    # Initialize table
    for i in range(26):
        if present[i]:
            nt = chr(ord('A') + i)
            table[nt] = {}
    
    # Fill table
    n = len(lhs)
    for i in range(n):
        A = lhs[i]
        prod = rhs[i]
        A_idx = ord(A) - ord('A')
        
        # Compute FIRST of production
        first_prod = set()
        
        if len(prod) == 0:
            first_prod.add('e')
        else:
            j = 0
            L = len(prod)
            while j < L:
                sym = prod[j]
                
                if not is_nt(sym):
                    first_prod.add(sym)
                    break
                else:
                    B = ord(sym) - ord('A')
                    for k in range(128):
                        if k == ord('e'):
                            continue
                        if firsts[B][k]:
                            first_prod.add(chr(k))
                    
                    if not firsts[B][ord('e')]:
                        break
                    
                    if j == L - 1:
                        first_prod.add('e')
                j += 1

        for terminal in first_prod:
            if terminal != 'e':
                if terminal in table[A]:
                    print(f"Conflict at [{A}, {terminal}]!")
                table[A][terminal] = prod
        
        if 'e' in first_prod:
            for k in range(128):
                if follows[A_idx][k]:
                    terminal = chr(k)
                    if terminal in table[A]:
                        print(f"Conflict at [{A}, {terminal}]!")
                    table[A][terminal] = prod

def display_table():
    """Display LL(1) parsing table"""
    print("\n=== LL(1) PARSING TABLE ===\n")
    
    terminals_sorted = sorted(terminals)
    
    print(f"{'':5}", end="")
    for t in terminals_sorted:
        print(f"{t:10}", end="")
    print()
    print("-" * (5 + 10 * len(terminals_sorted)))
    
    
    for i in range(26):
        if not present[i]:
            continue
        nt = chr(ord('A') + i)
        print(f"{nt:5}", end="")
        for t in terminals_sorted:
            prod = table[nt].get(t, '')
            output = f"{nt}->{prod}" if prod else ''
            print(f"{output:10}", end="")
        print()

def build_ll1_parser(productions):
    parse_grammar(productions)
    compute_first()
    compute_follow()
    construct_table()
    display_table()


if __name__ == "__main__":
    # Example 1: E -> TX, X -> +TX | e, T -> FY, Y -> *FY | e, F -> (E) | id
    productions1 = [
        "E TX",
        "X +TX",
        "X e",
        "T FY",
        "Y *FY",
        "Y e",
        "F (E)",
        "F id"
    ]
    
    build_ll1_parser(productions1)
    
