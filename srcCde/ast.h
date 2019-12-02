#ifndef _AST_H
#define _AST_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;



class ast_program;
class ast_field_decl;
class ast_var_decl;
class ast_id;
class ast_method_decl;
class ast_type_identifier;
class ast_statement;
class ast_expression;
class ast_block_statement;
class ast_assignment_statement;
class ast_method_call;
class ast_normal_method;
class ast_prnt_method;
class ast_prnt_arg;
class ast_string_prnt_arg;
class ast_expression_prnt_arg;
class ast_if_statement;
class ast_for_statement;
class ast_return_statement;
class ast_continue_statement;
class ast_break_statement;
class ast_location;
class ast_var_location;
class ast_array_location;
class ast_literal_expression;
class ast_integer_literal_expression;
class ast_char_literal_expression;
class ast_true_literal_expression;
class ast_false_literal_expression;
class ast_binary_operation_expression;
class ast_unary_operation_expression;

enum class AssignOp {
    plus_equal,
    minus_equal,
    equal
};

enum class bin_op {
    plus_op,
    minus_op,
    multiply_op,
    divide_op,
    modulo_op,
    lessthan_op,
    greaterthan_op,
    lessequal_op,
    greaterequal_op,
    notequal_op,
    equalequal_op,
    and_op,
    or_op
};

enum class Datatype {
    int_type,
    void_type,
    bool_type
};

enum class un_op {
    minus_op,
    not_op
};

union node{
  AssignOp assign_op;
  ast_block_statement * block;
  ast_prnt_arg * prnt_arg;
  ast_expression * expr;
  ast_field_decl * field_decl;
  ast_id * var_or_array_identifier;
  ast_literal_expression * literal;
  ast_location * location;
  ast_method_call * method_call;
  ast_method_decl * method_decl;
  ast_program * program;
  ast_statement * statement;
  ast_type_identifier * type_identifier;
  ast_var_decl * var_decl;
  char *sval;
  char cval;
  Datatype type;
  int ival;
  std::vector<ast_prnt_arg *> * prnt_arg_list;
  std::vector<ast_expression *> * expr_list;
  std::vector<ast_field_decl *> * field_decl_list;
  std::vector<ast_id *> * identifier_list;
  std::vector<ast_id *> * var_or_array_identifier_list;
  std::vector<ast_method_decl *> * method_decl_list;
  std::vector<ast_statement *> * statement_list;
  std::vector<ast_type_identifier *> * type_identifier_list;
  std::vector<ast_var_decl *> * var_decl_list;
};
typedef union node YYSTYPE;
#define YYSTYPE_IS_DECLARED 1

class ast_node {
public:
    ast_node() {}
    ~ast_node() {}
};

class ast_program : public ast_node
{
    string id;
    vector<ast_field_decl *> * fdl;
    vector<ast_method_decl *> * mdl;
public:
    ast_program(string, vector<ast_field_decl *> * , vector<ast_method_decl *> *);
    string get_id();
    vector<ast_method_decl *> * getMdl();
    vector<ast_field_decl *> * getFdl();
    ~ast_program() {}
};


class ast_field_decl : public ast_node
{
    Datatype type;
    vector<ast_id *> * id_list;
public:
    ast_field_decl(vector<ast_id *> *, Datatype);
    vector<ast_id *> * get_id_list();
    Datatype getType();
    ~ast_field_decl() {}
};

class ast_var_decl : public ast_node
{
    Datatype type;
    vector<ast_id *> * id_list;
public:
    ast_var_decl(vector<ast_id *> *, Datatype);
    vector<ast_id *> * get_id_list();
    Datatype getType();
    ~ast_var_decl() {}
};

class ast_id
{
    string id;
    int size;
public:
    ast_id(string, int);
    ast_id(string id);
    string get_id();
    int getSize();
    ~ast_id() {}
};

class ast_method_decl : public ast_node
{
    string id;
    Datatype returnType;
    vector<ast_type_identifier *> * arguments;
    ast_block_statement * block;
public:
    ast_method_decl(string id, Datatype, vector<ast_type_identifier *> *, ast_block_statement *);
    string get_id();
    Datatype getReturnType();
    vector<ast_type_identifier *> * get_arguments();
    ast_block_statement * get_block();
    ~ast_method_decl() {}
};

class ast_type_identifier : public ast_node
{
    string id;
    Datatype type;
public:
    ast_type_identifier(string, Datatype);
    ~ast_type_identifier() {}
    string get_id();
    Datatype getType();
};

class ast_statement : public ast_node
{
public:
    ast_statement() {}
    virtual ~ast_statement() {}
};


class ast_expression : public ast_node
{
public:
    ast_expression() {}
    virtual ~ast_expression() {}
};

class ast_block_statement : public ast_statement
{
    vector<ast_var_decl *> * id_list;
    vector<ast_statement *> * stmtlist;
public:
    ast_block_statement(vector<ast_statement *> * , vector<ast_var_decl *> * );
    vector<ast_var_decl *> * get_id_list();
    vector<ast_statement *> * getStmtlist();
    ~ast_block_statement() {}
};

class ast_assignment_statement : public ast_statement
{
    AssignOp op;
    ast_location * location;
    ast_expression * expr;
public:
    ast_assignment_statement(AssignOp, ast_location *, ast_expression *);
    AssignOp get_op();
    ast_location * getLocation();
    ast_expression * get_expr();
    ~ast_assignment_statement() {}
};

class ast_method_call : public ast_statement, public ast_expression
{
public:
    ast_method_call() { }
    virtual ~ast_method_call() {}
};

class ast_normal_method : public ast_method_call
{
    string id;
    vector<ast_expression *> * arguments;
public:
    ast_normal_method(string, vector<ast_expression *> * );
    string get_id();
    vector<ast_expression *> * get_arguments();
    ~ast_normal_method() {}
};

class ast_prnt_method : public ast_method_call
{
    string method_name;
    vector<ast_prnt_arg *> * arguments;
public:
    ast_prnt_method(string , vector<ast_prnt_arg *> * );
    string get_method_name();
    vector<ast_prnt_arg *> * get_arguments();
    ~ast_prnt_method() {}
};

class ast_prnt_arg : public ast_expression
{
public:
    ast_prnt_arg() {}
    virtual ~ast_prnt_arg() {}
};

class ast_string_prnt_arg : public ast_prnt_arg
{
    string argument;
public:
    ast_string_prnt_arg(string);
    string get_argument();
    ~ast_string_prnt_arg() {}
};

class ast_expression_prnt_arg : public ast_prnt_arg
{
    ast_expression * argument;
public:
    ast_expression_prnt_arg(ast_expression *);
    ast_expression * get_argument();
    ~ast_expression_prnt_arg() {}
};

class ast_if_statement : public ast_statement
{
    ast_expression * condition;
    ast_block_statement * if_block;
    ast_block_statement * else_block;
public:
    ast_if_statement(ast_expression *, ast_block_statement *, ast_block_statement *);
    ast_expression * get_condition();
    ast_block_statement * getIf_block();
    ast_block_statement * getElse_block();
    ~ast_if_statement() {}
};

class ast_for_statement : public ast_statement
{
    string id;
    ast_expression * init_condition;
    ast_expression * end_condition;
    ast_block_statement * block;
public:
    ast_for_statement(ast_expression *, ast_expression *, ast_block_statement *, string);
    string get_id();
    ast_expression * getInit_condition();
    ast_expression * getEnd_condition();
    ast_block_statement * get_block();
    ~ast_for_statement() {}
};

class ast_return_statement : public ast_statement
{
    ast_expression * expr;
public:
    ast_return_statement(ast_expression *);
    ast_expression * get_expr();
    ~ast_return_statement() {}
};

class ast_continue_statement : public ast_statement
{
public:
    ast_continue_statement() {}
    ~ast_continue_statement() {}
};

class ast_break_statement : public ast_statement
{
public:
    ast_break_statement() {}
    ~ast_break_statement() {}
};

class ast_location : public ast_expression
{
public:
    ast_location() {}
    virtual ~ast_location() {}
};

class ast_var_location : public ast_location
{
    string id;
public:
    ast_var_location(string);
    string get_id();
    ~ast_var_location() {}
};

class ast_array_location : public ast_location
{
    string id;
    ast_expression * index;
public:
    ast_array_location(string, ast_expression * );
    string get_id();
    ast_expression * get_index();
    ~ast_array_location(){}
};

class ast_literal_expression : public ast_expression
{
public:
    ast_literal_expression(){}
    virtual ~ast_literal_expression(){}
};

class ast_integer_literal_expression : public ast_literal_expression
{
    int value;
public:
    ast_integer_literal_expression(int);
    int get_value();
    ~ast_integer_literal_expression(){}
};

class ast_char_literal_expression : public ast_literal_expression
{
    char value;
public:
    ast_char_literal_expression(char);
    char get_value();
    ~ast_char_literal_expression(){}
};

class ast_true_literal_expression : public ast_literal_expression
{
public:
    ast_true_literal_expression(){}
    bool get_value()
    {
      return true;
    }
    ~ast_true_literal_expression(){}
};

class ast_false_literal_expression : public ast_literal_expression
{
public:
    ast_false_literal_expression(){}
    bool get_value(){
        return false;
    }

    ~ast_false_literal_expression(){}
};

class ast_binary_operation_expression : public ast_expression
{
    ast_expression * left;
    ast_expression * right;
    bin_op op;
public:
    ast_binary_operation_expression(ast_expression *, ast_expression *, bin_op);
    ast_expression * get_left();
    ast_expression * get_right();
    bin_op get_op();
    ~ast_binary_operation_expression()
    {

    }
};

class ast_unary_operation_expression : public ast_expression
{
    ast_expression * expr;
    un_op op;
public:
    ast_unary_operation_expression(ast_expression *, un_op);
    ast_expression * get_expr();
    un_op get_op ();
    ~ast_unary_operation_expression()
    {

    }
};

#endif
