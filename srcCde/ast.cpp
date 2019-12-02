#include "ast.h"

#include <bits/stdc++.h>

ast_program::ast_program(string id, vector<ast_field_decl *> * fdl, vector<ast_method_decl *> * mdl){
    auto astPrgId = id;
    auto methodDeclLst = mdl;
    auto funcDeclLst = fdl;
    this->id = astPrgId, this->mdl = methodDeclLst, this->fdl = funcDeclLst;
}

string ast_program::get_id()
{
    auto astID = this->id;
    return astID;
}

vector<ast_method_decl *> * ast_program::getMdl()
{
    auto methodDeclLst = this->mdl;
    return methodDeclLst;
}

vector<ast_field_decl *> * ast_program::getFdl()
{
   auto funcDeclLst = this->fdl;
   return funcDeclLst;
}


ast_field_decl::ast_field_decl(vector<ast_id *> * id_list, Datatype type)
{
  this->type = type, this->id_list = id_list;
}

vector<ast_id *> * ast_field_decl::get_id_list()
{
    auto idLst = this->id_list;
    return idLst;
}

Datatype ast_field_decl::getType()
{
    auto dtType = this->type;
    return dtType;
}

ast_var_decl::ast_var_decl(vector<ast_id *> * id_list, Datatype type)
{
    this->type = type, this->id_list = id_list;
}

Datatype ast_var_decl::getType()
{
    auto dtType = this->type;
    return dtType;
}

vector<ast_id *> * ast_var_decl::get_id_list()
{
    auto idLst = this->id_list;
    return idLst;
}


ast_id::ast_id(string id, int size)
{
    if (size <= 0) {
        cerr << "Invalid Size, has to be greater than zero" << endl;
        exit(-1);
    }
    auto astId = id;
    auto astSz = size;
    this->id = astId, this->size = astSz;
}

ast_id::ast_id(string id)
{
    auto astId = id;
    this->id = astId, this->size = 0;
}

string ast_id::get_id()
{
    auto astId = this->id;
    return astId;
}

int ast_id::getSize()
{
    auto astSz = this->size;
    return astSz;
}

ast_type_identifier::ast_type_identifier(string id, Datatype type)
{
    this->id = id, this->type = type;
}

string ast_type_identifier::get_id()
{
    auto astTypId = this->id;
    return astTypId;
}

Datatype ast_type_identifier::getType()
{
    auto dtType = this->type;
    return dtType;
}

ast_string_prnt_arg::ast_string_prnt_arg(string argument)
{
    auto strngArgs = argument;
    this->argument = strngArgs;
}

string ast_string_prnt_arg::get_argument()
{
    auto strngArgs = this->argument;
    return strngArgs;
}

ast_expression_prnt_arg::ast_expression_prnt_arg(ast_expression * argument)
{
    auto exprArgs = argument;
    this->argument = exprArgs;
}

ast_expression * ast_expression_prnt_arg::get_argument()
{
    auto exprArgs = this->argument;
    return exprArgs;
}

ast_var_location::ast_var_location(string id)
{
    this->id = id;
}
string ast_var_location::get_id()
{
    auto astVarId = this->id;
    return astVarId;
}


ast_array_location::ast_array_location(string id, ast_expression * index)
{
    this->id = id, this->index = index;
    // this->index = index;
}

string ast_array_location::get_id()
{
    auto astArId = this->id;
    return astArId;
}
ast_expression * ast_array_location::get_index()
{
    auto astExprArInd = this->index;
    return astExprArInd;
}

ast_integer_literal_expression::ast_integer_literal_expression(int value)
{
    this->value = value;
}
int ast_integer_literal_expression::get_value()
{
    auto intLtrlVal = this->value;
    return intLtrlVal;
}


ast_char_literal_expression::ast_char_literal_expression(char value)
{
    auto charVal = value;
    this->value = charVal;
}
char ast_char_literal_expression::get_value()
{
    auto charVal = this->value;
    return charVal;
}

ast_binary_operation_expression::ast_binary_operation_expression(ast_expression * left, ast_expression * right, bin_op op)
{
    this->left = left, this->right = right, this->op = op;
}
ast_expression * ast_binary_operation_expression::get_left()
{
    auto astExprLft = this->left;
    return astExprLft;
}
ast_expression * ast_binary_operation_expression::get_right()
{
    auto astExprRt = this->right;
    return astExprRt;
}
bin_op ast_binary_operation_expression::get_op()
{
    auto astBnrOp = this->op;
    return astBnrOp;
}

ast_unary_operation_expression::ast_unary_operation_expression(ast_expression * expr, un_op op)
{
    this->expr = expr, this->op = op;
    // this->op = op;
}
ast_expression * ast_unary_operation_expression::get_expr()
{
    auto astUnrExpr = this->expr;
    return astUnrExpr;
}
un_op ast_unary_operation_expression::get_op ()
{
    auto astUnrOp = this->op;
    return astUnrOp;
}
