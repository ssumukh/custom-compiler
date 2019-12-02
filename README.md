# Custom Compiler for a C-like language

Back-end of the compiler for a custom programming language, similar to C. 

Built parser, scanner, abstract syntax tree, interpreter and generates intermediate representation (LLVM IR) code for an input code file.

  The language design for the language is in [this](https://github.com/nonejk/custom-compiler/blob/master/compilers_language_design.pdf) file, where the grammar is explained in details, with the data types and operations supported, both unary and binary, supported; output format; control statements; and function declarations, along with semantic analysis.

Sample codes in this language can be found in [this](https://github.com/nonejk/custom-compiler/tree/master/exmplCdes) directory.

### Requirements:

flex

bison

### How to run:

Navigate to the ```srcCde``` directory

Run ```make``` command. This should create an executable named ```parser```

To get the IR for any of the example codes added in the ```exmpleCdes``` directory, do the following:

```./parser ../exmplCdes/<file name>```

The IR will be printed onto the STDOUT

### TODO: 

Improve readme
