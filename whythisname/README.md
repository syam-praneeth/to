# CDC Programs (CN + CD)

This folder contains C, Lex, and Yacc programs split out from `all.md`.

## Contents

CN (Computer Networks)
Original C sources:
 - bit_stuffing.c — Bit stuffing/unstuffing
 - byte_stuffing.c — Byte stuffing/destuffing with FLAG/ESC
 - hamming.c — Hamming distance for 4/8-bit strings
 - minham.c — Minimum Hamming distance among 8 codewords
 - checksum.c — Internet-style checksum (4-bit fold for demo)
 - crc.c — CRC generation/check
 - dvr.c — Distance Vector Routing
 - lsr.c — Link State Routing (Dijkstra)

Converted C++ versions (same logic, use std::string / std::vector):
 - bit_stuffing.cpp
 - byte_stuffing.cpp
 - hamming.cpp
 - minham.cpp
 - checksum.cpp
 - crc.cpp
 - dvr.cpp
 - lsr.cpp

CD (Compiler Design)
- lexical_analyzer.c — Simple token classifier
- lex.l — Lex program recognizing keywords/ids/ops
- expr.y + expr.l — Yacc+Lex: validate arithmetic expressions
- calc.y + calc.l — Yacc+Lex: calculator with error handling
- first_follow.c — FIRST and FOLLOW sets
- three_address.c — Three Address Code generation
- machine_code.c — Simple machine-code style generation via AST

## Build and run (Windows PowerShell)

Prerequisites:
- C compiler (GCC via MSYS2/MinGW, LLVM/Clang, or Visual Studio Build Tools)
- For Lex/Yacc: Flex and Bison (via MSYS2, Chocolatey, or WSL)

Examples using MSYS2 MinGW (recommended) or WSL. If you already have `gcc`, you can run these directly.

### Compile C-only programs

```powershell
# Compile and run (examples)
gcc bit_stuffing.c -o bit_stuffing.exe; .\bit_stuffing.exe
gcc byte_stuffing.c -o byte_stuffing.exe; .\byte_stuffing.exe
gcc hamming.c -o hamming.exe; .\hamming.exe
gcc minham.c -o minham.exe; .\minham.exe
gcc checksum.c -o checksum.exe; .\checksum.exe
gcc crc.c -o crc.exe; .\crc.exe
gcc dvr.c -o dvr.exe; .\dvr.exe
gcc lsr.c -o lsr.exe; .\lsr.exe

gcc lexical_analyzer.c -o lexical_analyzer.exe; .\lexical_analyzer.exe
gcc three_address.c -o three_address.exe; .\three_address.exe
gcc machine_code.c -o machine_code.exe; .\machine_code.exe

gcc first_follow.c -o first_follow.exe; .\first_follow.exe

# Compile C++ versions
g++ bit_stuffing.cpp -o bit_stuffing_cpp.exe; .\bit_stuffing_cpp.exe
g++ byte_stuffing.cpp -o byte_stuffing_cpp.exe; .\byte_stuffing_cpp.exe
g++ hamming.cpp -o hamming_cpp.exe; .\hamming_cpp.exe
g++ minham.cpp -o minham_cpp.exe; .\minham_cpp.exe
g++ checksum.cpp -o checksum_cpp.exe; .\checksum_cpp.exe
g++ crc.cpp -o crc_cpp.exe; .\crc_cpp.exe
g++ dvr.cpp -o dvr_cpp.exe; .\dvr_cpp.exe
g++ lsr.cpp -o lsr_cpp.exe; .\lsr_cpp.exe
```

### Build Lex/Yacc programs (Flex/Bison)

Option A: MSYS2 shell (install `flex` and `bison`) or WSL (Ubuntu: `sudo apt install flex bison build-essential`).

```bash
# expr (validator)
bison -d expr.y && flex expr.l && gcc lex.yy.c expr.tab.c -o expr
./expr

# calc (calculator)
bison -d calc.y && flex calc.l && gcc lex.yy.c calc.tab.c -o calc
./calc
```

On Windows PowerShell (with flex/bison in PATH), commands are the same but executables end with `.exe`.

### Notes
- For MSYS2, use the MSYS2 MinGW 64-bit shell for compiling (ensures correct headers/libs).
- If using Visual Studio compiler (`cl`), adapt flags accordingly.
- The Lex-only `lex.l` can be built with: `flex lex.l && gcc lex.yy.c -lfl -o lexprog` (on Windows you may not need `-lfl`).

## Inputs/Outputs quick tips
- Programs that read full expressions sometimes stop on newline; if input seems skipped, run them again and provide input as prompted.
- `lexical_analyzer.c` reads until a `$` character.
- `dvr.c` expects `-1` for no direct link in the cost matrix.

## Troubleshooting
- If `gcc` is not found: install MSYS2 or WSL, or add your compiler to PATH.
- If `flex`/`bison` are missing: install via MSYS2 (`pacman -S flex bison`) or WSL (`apt install flex bison`).
- On Windows, linking against `-lfl` may be unnecessary; if you see linker errors, try omitting `-lfl`.
