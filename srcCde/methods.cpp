#include "ast.h"

#include <bits/stdc++.h>



ast_method_decl::ast_method_decl(string id, Datatype returnType, vector<ast_type_identifier *> * arguments, ast_block_statement * block)
{
    this->id = id, this->returnType = returnType, this->arguments = arguments, this->block = block;
}

string ast_method_decl::get_id()
{
    auto mthdId = this->id;
    return mthdId;
}

Datatype ast_method_decl::getReturnType()
{
    auto mthdRtrn = this->returnType;
    return mthdRtrn;
}

vector<ast_type_identifier *> * ast_method_decl::get_arguments()
{
    auto astArgs = this->arguments;
    return astArgs;
}

ast_block_statement * ast_method_decl::get_block()
{
    auto mthdBlk = this->block;
    return mthdBlk;
}

ast_normal_method::ast_normal_method(string id, vector<ast_expression *> * arguments)
{
    this->id = id, this->arguments = arguments;
}

string ast_normal_method::get_id()
{
    auto astId = this->id;
    return astId;
}

vector<ast_expression *> * ast_normal_method::get_arguments()
{
    auto mthdArgs = this->arguments;
    return mthdArgs;
}

ast_prnt_method::ast_prnt_method(string method_name, vector<ast_prnt_arg *> * arguments)
{
    this->method_name = method_name, this->arguments = arguments;
}

string ast_prnt_method::get_method_name()
{
    auto mthdNme = this->method_name;
    return mthdNme;
}

vector<ast_prnt_arg *> * ast_prnt_method::get_arguments()
{
    auto prntArgs = this->arguments;
    return prntArgs;
}
