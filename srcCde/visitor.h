#ifndef _visitor_H
#define _visitor_H

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


class Visitor
{
public:
    virtual void * visit(ast_program *) = 0;
    virtual void * visit(ast_field_decl *) = 0;
    virtual void * visit(ast_var_decl *) = 0;
    virtual void * visit(ast_id *) = 0;
    virtual void * visit(ast_method_decl *) = 0;
    virtual void * visit(ast_type_identifier *) = 0;
    virtual void * visit(ast_statement *) = 0;
    virtual void * visit(ast_expression *) = 0;
    virtual void * visit(ast_block_statement *) = 0;
    virtual void * visit(ast_assignment_statement *) = 0;
    virtual void * visit(ast_method_call *) = 0;
    virtual void * visit(ast_normal_method *) = 0;
    virtual void * visit(ast_prnt_method *) = 0;
    virtual void * visit(ast_prnt_arg *) = 0;
    virtual void * visit(ast_string_prnt_arg *) = 0;
    virtual void * visit(ast_expression_prnt_arg *) = 0;
    virtual void * visit(ast_if_statement *) = 0;
    virtual void * visit(ast_for_statement *) = 0;
    virtual void * visit(ast_return_statement *) = 0;
    virtual void * visit(ast_continue_statement *) = 0;
    virtual void * visit(ast_break_statement *) = 0;
    virtual void * visit(ast_location *) = 0;
    virtual void * visit(ast_var_location *) = 0;
    virtual void * visit(ast_array_location *) = 0;
    virtual void * visit(ast_literal_expression *) = 0;
    virtual void * visit(ast_integer_literal_expression *) = 0;
    virtual void * visit(ast_char_literal_expression *) = 0;
    virtual void * visit(ast_true_literal_expression *) = 0;
    virtual void * visit(ast_false_literal_expression *) = 0;
    virtual void * visit(ast_binary_operation_expression *) = 0;
    virtual void * visit(ast_unary_operation_expression *) = 0;
};

#endif
