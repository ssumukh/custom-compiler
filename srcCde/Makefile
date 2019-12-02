FLAGS=-O3 -flto -Wall -std=c++14 `llvm-config --cppflags --ldflags --system-libs --libs core`

parser: parser.y scanner.l ast.h ast.cpp methods.cpp statements.cpp symbol_table.h visitor.h codegen_visitor.h statementgen_visitor.h expressiongen_visitor.h
		bison -d parser.y
		flex scanner.l
		g++ -o parser main.cpp ast.cpp methods.cpp statements.cpp lex.yy.c parser.tab.c -lfl $(FLAGS) -g -w

.PHONY: clean
clean:
		rm -rf *.o
