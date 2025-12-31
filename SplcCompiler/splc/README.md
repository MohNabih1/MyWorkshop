# SPLC: A compiler from a Simple Programming Language (SPL) to LLVM

It requires the Unix packages: make, gcc, g++, flex, bison, libreadline-dev, libgc-dev, and llvm.
To build SPLC, run `make`.

Commands:

| command   | explanation                   |
|-----------|-------------------------------|
| bin/splc tests/hanoi.spl | compile this file to LLVM tests/hanoi.ll |
| bin/run tests/hanoi.ll  | compile this LLVM file to a.out and run it |
| bin/splc -eval tests/hanoi.spl | run this file using the SPL interpreter |
| bin/splc | use the REPL interpreter |

Other parameters to `bin/splc`:

| parameter | action |
|-----------|-----------|
| -trace scanner | trace the scanner |
| -trace parser | trace the parser |
| -trace typecheck | trace the type checker |
| -trace codegen | trace the IR code generator |
| -trace eval | trace the interpreter |
| -show ast | print the AST |
| -show ir | print the IR |
