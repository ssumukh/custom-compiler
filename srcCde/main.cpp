#include <bits/stdc++.h>

#include "ast.h"
#include "codegen_visitor.h"
#include "visitor.h"

extern "C" FILE *yyin;
extern "C" int yyparse();

ast_program * start;

int main (const int argc, const char ** argv) {
    bool inpCr = argc >= 2;
    if (inpCr) {
        auto inpFl = argv[1];
        FILE *infile = fopen(inpFl, "r");
        if (!infile) {
            std::cerr <<"Error reading file " << inpFl << "\nEnding process" << std::endl;
            return -1;
        }
        yyin = infile;
        if (!yyparse()) {
            codegen_visitor * visitor = new codegen_visitor(start);
            visitor->code_gen();
        }    
    } else {
        yyin = NULL;
        std::cerr <<"fatal error: no input files" << std::endl;
        return -1;
    }

    return 0;
}
